
var FILE = require("file");

use("jake/c");

exports.cFlags = [
    '-ansi',
    '-pedantic',
    '-O3',
    '-Wall'
];
if (system.os != "Mac OS X") {
    exports.cFlags.push(
        '-static',
        '-static-libgcc'
    );
}

task("default", "fw");

task("clean", function () {
    jakefile.path.dirname().glob("*.o").forEach(FILE.remove);
    jakefile.path.dirname().join("fw").remove();
});

