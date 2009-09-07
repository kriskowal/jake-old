
var UTIL = require("util");
var PATTERN = require("./pattern");
var FileTask = require("./filetask").FileTask;

var Rule = exports.Rule = function (pattern, context) {
    this.pattern = PATTERN.pattern(pattern, context.path);
    this.context = context;
};

Rule.prototype.depends = function (depender) {
    if (UTIL.isArrayLike(depender)) {
        var depends = depender;
        depender = function (basePath) {
            return depends.map(function (path) {
                return basePath.resolve(path);
            });
        };
    }
    this.context.dependers.unshift([this.pattern, depender]);
    return this;
};

Rule.prototype.dep = Rule.prototype.depends;

Rule.prototype.build = function (builder) {
    this.context.builders.unshift([this.pattern, builder]);
    return this;
};

Rule.prototype.action = Rule.prototype.build;

