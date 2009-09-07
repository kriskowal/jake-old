
var FILE = require("file");

use("jake/c");

if (system.os == "Mac OS X") {
    exports.cFlags = [
        '-Wall',
        '-O3',
        '-I/Developer/SDKs/MacOSX10.5.sdk/usr/include',
        '-L/Developer/SDKs/MacOSX10.5.sdk/usr/lib'
    ];
} else {
    exports.cFlags = [
        '-ansi',
        '-pedantic',
        '-O3',
        '-static',
        '-static-libgcc',
        '-Wall',
        '-O3'
    ];
}

task("default", "fw");

task("clean", function () {
    jakefile.path.dirname().glob("*.o").forEach(FILE.remove);
    jakefile.path.dirname().join("fw").remove();
});

