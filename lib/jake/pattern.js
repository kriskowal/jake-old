
var FILE = require("file");
var UTIL = require("util");

exports.pattern = function (pattern) {
    if (typeof pattern == "string")
        return new GlobPattern(pattern);
    if (pattern.constructor === RegExp)
        return new RegExpPattern(pattern);
    if (!pattern.match)
        throw new Error("Unrecognized pattern matcher: " + UTIL.repr(pattern));
    return pattern;
};

var ExactPattern = exports.ExactPattern = function (pattern) {
    this.pattern = pattern;
};

ExactPattern.prototype.match = function (path) {
    return this.pattern == path;
};

var GlobPattern = exports.GlobPattern = function (pattern) {
    this.pattern = pattern;
};

GlobPattern.prototype.match = function (path) {
    return FILE.match(path, this.pattern);
};

var RegExpPattern = exports.RegExpPattern = function (pattern) {
    this.pattern = pattern;
};

RegExpPattern.prototype.match = function (path) {
    return new RegExp('^' + this.pattern.source + '$').test(path);
};

