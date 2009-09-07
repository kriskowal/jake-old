
var FILE = require("file");

with(require("jake")) {

    // circular
    task("circular").dep("foo1");
    task("foo1").dep(["foo2"]);
    task("foo2").dep(["foo1"]);

    // example
    task("build").dep(["generate_html", "copy_images"]);
    task("generate_html").dep(["create_directories"]).action(function() {
        print("generate_html!");
    });
    task("copy_images").dep(["create_directories"]).action(function() {
        print("copy_images!");
    });
    task("create_directories").action(function() {
        print("create_directories!");
    });

    file("foobar").dep(["barfoo"]).action(function() {
        print("writing");
        FILE.write("foobar", "baz");
    })

    file("barfoo").action(function() {
        print("barfoo");
    });

    directory("barrr/baz/foo");

    task("fooo").dep("Jakefile")

    // defaut
    task("default").dep(["build", "fooo", "barrr/baz/foo"]);


    task("a1").dep("b1").action(function() {
        print("a1")
    });
}
