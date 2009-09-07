
var UTIL = require("util");

var Task = exports.Task = function(name, context) {
    this.name = name;
    this.context = context;
    this.dependencies = [];
    this.actions = [];
}

Task.prototype.depends = function (dependency) {
    var self = this;
    Array.prototype.forEach.call(arguments, function (arg) {
        if (UTIL.isArrayLike(arg))
            self.dependencies.push.apply(self.dependencies, arg);
        else
            self.dependencies.push(arg);
    });
    return self;
}

Task.prototype.dep = function(dependency) {
    this.dependencies = this.dependencies.concat(dependency);
    return this;
}

Task.prototype.action = function(action) {
    this.actions.push(action);
    return this;
}

Task.prototype.invoke = function() {
    var self = this;
    this.dependencies.forEach(function (dependency) {
        self.context.process.deepBuild(self.context.path.dirname().join(dependency));
    });
    this.actions.forEach(function(action) {
        action.call(this);
    }, this);
}

