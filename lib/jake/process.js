
var FILE = require("file");
var OS = require("os");
var UTIL = require("util");
var CONTEXT = require("./context");
var stream = require('term').stream;

// captures all of the state for the process of performing
// any tasks, dependency analysis, or building, in multiple
// directory contexts (trees)
var Process = exports.Process = function () {
    this.contexts = {};
};

// returns the shallow dependencies of a file denoted
//  with a given path
// the context argument is used internally to improve
//  performance by minimizing wasteful context searches
Process.prototype.depends = function (path, context) {
    if (!context)
        context = this.context(path);
    path = FILE.path(path).canonical();
    while (context.exports) {
        var basePath = context.path.dirname().join('');
        var dependers = context.dependers;
        var length = dependers.length;
        for (var i = 0; i < length; i++) {
            var depender = dependers[i];
            var matcher = depender[0];
            if (matcher.match(path.from(basePath))) {
                return depender[1].call(this, path);
            }
        }
        context = context.parent;
    };
    return [];
};

// returns the transitive dependencies for a given
//  path
// the visited argument is an internal object for tracking
//  that nodes have already been visited, for both reporting
//  and cycle-breaking
// the context argument is used internally to improve
//  performance by minimizing wasteful context searches
Process.prototype.deepDepends = function (path, visited, context) {
    var self = this;
    if (!context)
        context = self.context(path);
    if (!visited)
        visited = {};
    if (UTIL.has(visited, path))
        return [];
    visited[path] = true;
    var depends = self.depends(path, context);
    if (UTIL.no(depends) || !depends.forEach)
        throw new Error("dependencies of " + UTIL.repr(path) + " were not an array");
    depends.forEach(function (dependee) {
        self.deepDepends(dependee, visited, context);
    });
    return Object.keys(visited).map(function (key) {
        return FILE.path(key);
    });
};

// invokes the builder of a file at a given path
// the context argument is used internally to improve
//  performance by minimizing wasteful context searches
Process.prototype.build = function (path, context, depth) {
    if (!context)
        context = this.context(path);
    if (!depth)
        depth = 0;
    path = FILE.path(path).canonical();
    while (context.exports) {
        var basePath = context.path.dirname().join('');
        var builders = context.builders;
        var length = builders.length;
        for (var i = 0; i < length; i++) {
            var builder = builders[i];
            var matcher = builder[0];
            if (matcher.match(path.from(basePath))) {
                return builder[1].call(this, path);
            }
        }
        context = context.parent;
    };
    return [];
};

// builds all of a file's transitive dependencies, if they have changed,
// finally building itself
Process.prototype.deepBuild = function (path, visited, context, depth) {
    var self = this;
    if (!depth)
        depth = 0;
    if (!context)
        context = self.context(path);
    if (!visited)
        visited = {};
    path = FILE.path(path).canonical();
    if (UTIL.has(visited, path))
        return [];
    visited[path] = true;
    var depends = self.depends(path, context);
    if (UTIL.no(depends) || !depends.forEach)
        throw new Error("dependencies of " + UTIL.repr(path) + " were not an array");
    if (this.verbose)
        this.print(
            "\0green(" +
            UTIL.mul("\\", depth + 1) + " " +
            path.relative() + 
            "\0)"
        );
    depends.forEach(function (dependee) {
        self.deepBuild(dependee, visited, context, depth + 1);
    });
    if (this.verbose)
        this.print(
            "\0green(" + 
            UTIL.mul("/", depth + 1) + " " +
            path.relative() + 
            "\0)"
        );

    if (self.force || self.isNeeded(path)) {
        self.build(path, context, depth);
    } else {
        if (self.verbose)
            self.print("\0yellow(Not changed.\0)");
    }

};

// returns whether the file at a given path needs to be built.
// this generally means that one of its shallow dependencies
// has changed.  Changes in transitive dependencies will trickle
// by touching the mtime of one of the shallow dependencies.
Process.prototype.isNeeded = function (path) {
    if (!path.exists())
        return true;
    var mine = path.mtime();
    return UTIL.some(this.depends(path), function (path) {
        return path.mtime() > mine;
    });
};

// returns a task function for a given name and path.
// this is a simple lookup since we take advantage of
// prototypical inheritance on the tasks object.
Process.prototype.lookupTask = function (path, name, context) {
    if (!context)
        context = this.context(path);
    return context.tasks[name];
};

// returns a context for an exact configuration file.
Process.prototype.config = function (path) {
    path = FILE.path(path).canonical();
    if (!UTIL.has(this.contexts, path)) {
        var context = this.contexts[path] = new CONTEXT.Context(path, this);
        context.factory(context);
    }
    return this.contexts[path];
};

// returns the context for any non-configuration file,
// by searching for Jakefiles in its directory heredity.
Process.prototype.context = function (path) {
    path = FILE.path(path).resolve('.');
    var paths = path.globPaths(".../Jakefile{,.js}");
    if (!paths.length)
        return {};
    else
        return this.config(paths.shift());
};

// TODO this could be overridden someday for hash-based
// changes
Process.prototype.touch = function (path) {
    path.touch();
};

// wraps the system call with verbose messages and
// echoing
Process.prototype.system = function (command) {
    var commandDisplay = command;
    if (UTIL.isArrayLike(commandDisplay))
        commandDisplay = commandDisplay.map(function (term) {
            if (/ /.test(term))
                return OS.enquote(term);
            return term;
        }).join(" ");
    this.print("\0blue(" + commandDisplay + "\0)");
    this.write("\0red(");
    var status = OS.system(command);
    this.write("\0)");
    if (status) {
        this.print("\0red(Status: " + status + "\0)");
        this.abort();
    }
};

Process.prototype.print = function () {
    return stream.print.apply(null, arguments);
};

Process.prototype.write = function () {
    return stream.write.apply(null, arguments);
};

Process.prototype.flush = function () {
    return stream.flush.apply(null, arguments);
};

Process.prototype.abort = function () {
    this.print("\0red(Aborting.\0)");
    this.exit(-1);
};

Process.prototype.exit = function (status) {
    OS.exit(status);
};

