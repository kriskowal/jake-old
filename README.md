
Jake
====

A build system implemented in JavaScript, using a JavaScript
DSL (Domain Specific Language) for declaring and applying
build configuration information including tasks, builders,
dependencies, and dependency scanners.

A `Jakefile` (or `Jakefile.js`) is a JavaScript file that gets
evaluated in a special context with the following free
variables.  `jake` implicitly loads all of the Jakefiles
responsible for the files involved in a build process.
The build rules for a given file can be defined or used
by the Jakefiles in any of the parent directories of that
file, giving precedence to those rules defined nearest and last.
Dependency scanners and builders can be defined independently,
and patterns are defined in the Jakefile context, which gives
a Jakefile the ability to be both a generalist and a specialist.
Furthermore, general purpose Jakefile can be "used" in any
context.

These are the functions available in the Jakefile DSL:

 * `task(name, ...)`

    Defines a build task like "default" or
    "clean".  The "default" task is special,
    as it is used when "jake" is executed in a
    directory containing this Jakefile without
    arguments.

    Apart from the name, subsequent arguments
    are interpreted as dependencies or actions.

     * a String is interpreted as a literal 
       dependency of the task.

     * an Array of Strings are also read
       as a collection of depended paths.

     * a Function is interpreted as an action
       that must be called for the task.

    The returned task object can be further
    configured with the following chained
    member functions.

     * `.depends(dependencies...)`

        Adds dependencies, either Strings
        or Arrays of Strings, variadically.

     * `.action(block)`

        Adds an action function to the task.

 * `rule(pattern, ...)`

    Defines a rule for building files matching
    a particular pattern in the applied context.
    The returned Rule object is not itself
    used by the build processor, but can be used
    to add dependencies, dependency scanners,
    or build functions to particular patterns to
    their respective lookup tables.

    The first argument is the pattern to match.
    It may be a String, in which case it is interpreted
    as a glob expression; a RegExp; or an object
    that implements the .match(path) function.  The
    given path will be the path relative to
    the current context directory.  Note that this means
    that `"*"` and `"**"` behave differently in
    glob expressions, where `"*"` will only match
    files in the exact same directory as the
    Jakefile.

    Subsequent arguments are interpreted as
    dependencies or build functions.  Functions
    call `.build(block)` and other objects
    call `.depends(dependencies)`

     * `.depends(dependencies...)`

        Declares a dependency scanner
        function, or explicit dependencies.
        These may be variadic.

        A Function is interpreted as a dependency
        scanner function, which receives the
        canonical Path object of the target file
        and must return an Array of Paths for
        each shallow dependency.

        Strings are interpreted as glob expressions,
        Arrays of Strings as literal dependencies,
        and regular expressions as file matchers.

     * `.build(block)`

        Declares a build function for files matching
        the rule pattern.  The build function
        receives the canonical Path object of the
        file to build.  "this" in the build function
        is the build process object, which has
        methods like "touch", "system", and "print".

 * `file(path, ...)`

    Declares a rule for an exact file.  In all other ways,
    this function behaves identically to `rule`.

 * `directory(path)`

    Constructs rules to build the given directory tree.

 * `depends(path)`

    returns an array of Path objects for each
    shallow dependency of a file.

 * `deepDepends(path)`

    returns an array of Path objects for each
    transitive dependency of a file (a file, its
    dependencies, their dependencies, and so on).

 * `build(path)`

    builds a file and its transitive dependencies,
    if needed, respecting the --force flag if provided.

 * `use(id)`

    Applies another Jakefile, identified by a 
    CommonJS module identifier (like `"./config/Jakefile"`,
    or `"jake/c"`) to the current context.

 * `exports`

    An object that contains your exports, and
    prototypically inherits the exports of Jakefiles
    in parent directories.

 * `require(id)`

    Returns a CommonJS module.

 * `system`

    The CommonJS system module.

 * `print`

    The Narwhal print method.

 * `jakefile`

    The context object corresponding to this Jakefile.

     * `path`

        A Path object corresponding to this Jakefile.

     * `process`

        The root object of a build process, having the following
        attributes:

         * `system`

            A wrapper for `os.system` that echoes commands
            and aborts if any return a nonzero exit status code.

         * `touch(path)`

            A wrapper for `file.touch` that may be overridden
            at a later date to purge the last known hash of
            the corresponding file, notifying the build process
            that the file has changed and its dependees 
            must be rebuilt.

            Presently, `jake` compares modification time stamps.

         * `verbose`

            A boolean flag, set by the `-v`, `--verbose`, `-t`
            or `--trace` command line options, that indicates
            that verbose trace messages should be printed
            as each file is visited and completed by the build
            process.

         * `force`

            A boolean flag, set by the `-f` or `--force` command
            line flags indicates that all files are stale and
            should be rebuit.

         * `abort()`

            Prints an abort message and exits with a `-1`
            status code.

         * `exit(status)`

            A wrapper for `os.exit` that might be overriden
            to throw an exception instead of exiting
            for unit testing `jake` itself.

        And wrappers for `term.stream`, which accepts a DSL
        for terminal codes for color and such.

         * `print(...)`

         * `write(text)`

         * `flush()`

