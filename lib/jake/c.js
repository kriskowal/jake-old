
var FILE = require("file");
var UTIL = require("util");

rule("**.[ch]")
    .depends(function (path) {
        var depends = [];
        if (!path.isFile())
            throw new Error("path: " + path + " does not exist.");
        path.open().forEach(function (line) {
            if (/#include\s/.test(line)) {
                var match = line.match(/"([^"]+)"/);
                if (match)
                    depends.push(path.resolve(match[1]));
            }
        });
        return depends;
    }).build(function (path) {
        this.touch(path);
    });

rule("**.o")
    .depends(function (path) {
        var dirname = path.dirname();
        var basename = path.basename('.o');
        var c = dirname.join(basename + '.c');
        var cpp = dirname.join(basename + '.cpp');
        if (c.isFile())
            return [c];
        if (cpp.isFile())
            reutrn [cpp];
        throw new Error("No source file for " + path);
    })
    .build(function (path) {
        this.system(
            ["gcc"]
            .concat(exports.cFlags)
            .concat(['-c'])
            .concat(depends(path).map(function (path) {
                return path.relative();
            }))
            .concat(['-o', path.relative()])
        );
    });

// binaries (no extension)
rule(/^([^\/]*\/)*[^\.]+$/)
    .depends(function (path) {
        var depends = deepDepends(path + '.c')
        .map(function (path) {
            return path.dirname().join(
                path.basename(path.extension()) + ".o"
            );
        })
        return UTIL.unique(depends);
    })
    .build(function (path) {
        this.system(
            ["gcc"]
            .concat(exports.cFlags)
            .concat(
                deepDepends(path)
                .filter(function (path) {
                    return /\.o$/.test(path);
                })
                .map(function (path) {
                    return path.relative();
                })
            )
            .concat(['-o', path])
        )
    })

