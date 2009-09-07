
var UTIL = require("util");
var FILE = require("file");
var TASK = require("./task");
var RULE = require("./rule");
var PATTERN = require("./pattern");

// creates a new context, that inherits from its own
// context in the process.
// The context object loads and executes its the given
// configuration file, by path.
// Uses the give process to find and load its parent
// context.
// Tasks and exports are prototypeical children of the
// respective members of the parent context.
// Builders and dependency scanners are ordered and keyed
// on the context path, and a file matcher that can be
// any object supporting the ".match(file)" method, but
// typically match either literal file names, glob expressions
// or regular expressions.
var Context = exports.Context = function (path, process) {
    this.path = path;
    this.process = process;
    this.parent = process.context(path.dirname());
    this.dependers = [];
    this.builders = [];
    this.tasks = Object.create(this.parent.tasks);
    this.exports = Object.create(this.parent.exports);
    this.factory = this.evaluate();
};

// creates and adds a task to the context
Context.prototype.task = function (name) {
    var args = UTIL.array(arguments);
    var name = args.shift();
    var task = new TASK.Task(name, this);
    args.forEach(function (arg) {
        if (typeof arg == "string")
            task.depends(arg);
        else if (UTIL.isArrayLike(arg))
            task.depends(arg);
        else if (arg instanceof Function)
            task.action(arg);
        else
            throw new Error(
                "Unrecognized argument to 'task' " +
                name + ": " +
                UTIL.repr(arg)
            );
    });
    this.tasks[name] = task;
    return task;
};

// creates a rule for a given pattern.  String
// patterns are interpreted as glob expressions,
// regular expressions as RegExpPatterns, and
// other objects must implement "match(path)" to 
// define their behavior.
//
// The returned Rule object can be used to chain-
// define dependencies, dependency scanners,
// or builder functions for the given pattern.
// The resultant objects are attached to the
// context, not the rule, so this is purely
// a syntax construct for convenience.
//
// Further arguments are interpreted as
// either dependencies or build functions.
//
//   string -> dependency
//   array -> dependencies
//   function -> builder
//
Context.prototype.rule = function (pattern) {
    var args = UTIL.array(arguments);
    var pattern = args.shift();
    var rule = new RULE.Rule(pattern, this);
    args.forEach(function (arg) {
        if (typeof arg == "string")
            rule.depends([arg]);
        else if (UTIL.isArrayLike(arg))
            rule.depends(arg);
        else if (arg instanceof Function)
            rule.build(arg);
        else
            throw new Error("Unrecognized argument to 'rule': " + UTIL.repr(arg));
    });
    return rule;
};

// returns a rule that matches an exact 
// file name.  further arguments are treated
// exactly the same as the rule function.
Context.prototype.file = function (path) {
    var args = UTIL.array(arguments);
    var pattern = args.shift();
    var rule = new RULE.Rule(new PATTERN.ExactPattern(pattern), this);
    args.forEach(function (arg) {
        if (UTIL.isArrayLike(arg))
            rule.depends(arg);
        else if (arg instanceof Function)
            rule.build(arg);
        else
            throw new Error("Unrecognized argument to 'file': " + UTIL.repr(arg));
    });
    return rule;
};

// creates rules that construct each of the parts
// of a directory chain starting in the context path.
Context.prototype.directory = function(path) {
    var components = FILE.split(path),
        path = null;
    while (components.length) {
        path = path ? FILE.join(path, components.shift()) : components.shift();
        this.file(path).build(function (p) {
            p.mkdirs();
        });
    }
};

// used by the Context constructor to load the corresponding
// configuration Jakefile.  returns a function that can be applied
// on any context.  This function is used to build the context
// object for its own path, and also applied to any context
// that invokes it with a "use" directive.
Context.prototype.evaluate = function () {
    var self = this;
    var text = self.path.read();

    var require2 = function (id) {
        id = require.loader.resolve(id, self.path);
        return require(id);
    };

    if (system.engine == "rhino") {
        var context = Packages.org.mozilla.javascript.Context.getCurrentContext();
        var factory = context.compileFunction(
            system.global,
            (
                "function(" + [
                    "exports",
                    "jakefile",
                    "require",
                    "system",
                    "print",
                    "use",
                    "depends",
                    "deepDepends",
                    "build",
                    "rule",
                    "file",
                    "directory",
                    "task"
                ].join(",") + ")" +
                "{"+text+"\n// */\n}"
            ),
            self.path,
            1,
            null
        );
    } else {
        var factory = new Function(
            "exports",
            "jakefile",
            "require",
            "system",
            "print",
            "use",
            "depends",
            "deepDepends",
            "build",
            "rule",
            "file",
            "directory",
            "task",
            text
        );
    }

    return function (context) {

        var use = function (id) {
            id = require.loader.resolve(id, self.path);
            var path = require.loader.find(id)[1];
            self.process.config(path).factory(self);
        };

        return factory(
            context.exports,
            self,
            require2,
            system,
            print,
            use,
            self.process.depends.bind(self.process),
            self.process.deepDepends.bind(self.process),
            self.process.deepBuild.bind(self.process),
            context.rule.bind(context),
            context.file.bind(context),
            context.directory.bind(context),
            context.task.bind(context)
        );

    };

};

