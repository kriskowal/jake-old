
var UTIL = require("util");
var FILE = require("file");
var TASK = require("./task");
var RULE = require("./rule");
var PATTERN = require("./pattern");

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

Context.prototype.rule = function (pattern) {
    var args = UTIL.array(arguments);
    var pattern = args.shift();
    var rule = new RULE.Rule(pattern, this);
    args.forEach(function (arg) {
        if (UTIL.isArrayLike(arg))
            rule.depends(arg);
        else if (arg instanceof Function)
            rule.build(arg);
        else
            throw new Error("Unrecognized argument to 'rule': " + UTIL.repr(arg));
    });
    return rule;
};

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

