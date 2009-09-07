
var UTIL = require("util");

var Task = exports.Task = function(name, context) {
    this.name = name;
    this.context = context;
    this.dependencies = [];
    this.actions = [];
};

Task.prototype.dep = function() {
    print("Task method 'dep' is deprecated. Please use 'depends'.");
    return this.depends.apply(this, arguments);
};

Task.prototype.depends = function() {
    for (var i = 0; i < arguments.length; i++)
        this.dependencies = this.dependencies.concat(arguments[i]);
    return this;
};

Task.prototype.action = function(action) {
    this.actions.push(action);
    return this;
};

Task.prototype.invoke = function() {
    var self = this;
    this.dependencies.forEach(function (dependency) {
        self.context.process.deepBuild(self.context.path.dirname().join(dependency));
    });
    this.actions.forEach(function(action) {
        action.call(this);
    }, this);
};

