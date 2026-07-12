#define NOB_IMPLEMENTATION
#include "lib/nob.h"
#define FLAG_IMPLEMENTATION
#include "lib/flag.h"

Nob_Cmd cmd = {0};

int main(int argc, char **argv)
{
    GO_REBUILD_URSELF(argc, argv);

    bool run = false;
    flag_bool_var(&run, "run", false, "Run the program after compilation.");
    bool debug = false;
    flag_bool_var(&debug, "debug", false, "Run the program with GDB after compilation.");

    if (!flag_parse(argc, argv)) {
        flag_print_error(stderr);
        return 1;
    }

    if (run && debug) {
        fprintf(stderr, "Both -run and -debug provided\n");
        return 1;
    }

    if (!mkdir_if_not_exists(".build")) return 1;

    nob_cc(&cmd);
    cmd_append(&cmd, "-c");
    nob_cc_inputs(&cmd, "src/main.c");
    nob_cc_output(&cmd, ".build/main.o");
    nob_cc_flags(&cmd);
    cmd_append(&cmd, "-I./lib/", "-I./src/");
    if (debug) {
        cmd_append(&cmd, "-ggdb");
    }
    if (!nob_cmd_run(&cmd)) return 1;

    nob_cc(&cmd);
    nob_cc_inputs(&cmd, ".build/main.o");
    nob_cc_output(&cmd, "tech_support");
    cmd_append(&cmd, "-L./lib/", "-l:libraylib.a", "-lGL", "-lm", "-lpthread", "-ldl", "-lX11");
    if (!nob_cmd_run(&cmd)) return 1;

    if (run) {
        cmd_append(&cmd, "./tech_support");
        if (!cmd_run(&cmd)) return 1;
    } else if (debug) {
        cmd_append(&cmd, "gdb", "-ex", "run", "--args", "./tech_support");
        if (!cmd_run(&cmd)) return 1;
    }

    return 0;
}
