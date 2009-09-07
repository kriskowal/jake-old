
var ARGS = require("args");
var UTIL = require("util");
var FILE = require("file");
var PROCESS = require("./jake/process");

var parser = exports.parser = new ARGS.Parser();

parser.option('-c', '--jakefile', 'configFile')
    .help("Explicitly specify a configuration file or context")
    .set();
parser.option('-v', '-t', '--trace', '--verbose', 'verbose')
    .help("Trace the dependency tree traversal.")
    .set(true);
parser.option('-f', '--force', 'force')
    .help("Build all dependencies, regardless of whether they have changed.")
    .set(true);
parser.option('-T', '--show-tasks', 'showTasks')
    .help("List all of the configured tasks.")
    .set(true)
    .halt();

parser.helpful();

exports.main = function (_system) {
    if (!_system)
        _system = system;

    var process = new PROCESS.Process();

    parser.parse(_system.args, process);

    // load the Jakefiles for the current directory,
    //  so that tasks are installed
    var context = process.configFile ?
        FILE.path(process.configFile) :
        FILE.path(FILE.cwd(), '');
    process.context(context);

    if (process.showTasks) {
        var context = process.context(context);
        while (context.tasks) {
            Object.keys(context.tasks).forEach(function (task) {
                print(task);
            });
            context = context.parent;
        }
        return 0;
    }

    if (_system.args.length) {
        _system.args.forEach(function (arg) {
            var task = process.lookupTask(context, arg);
            if (task) {
                task.invoke();
            } else {
                process.deepBuild(arg);
            }
        })
    } else {
        var task = process.lookupTask(context, 'default');
        if (task) {
            task.invoke();
        } else {
            process.print("\0red(No default task defined.\0)");
        }
    }

};

if (require.main == module.id)
    require("os").exit(exports.main());

