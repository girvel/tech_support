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

    if (!flag_parse(argc, argv)) {
        flag_print_error(stderr);
        return 1;
    }

    if (!mkdir_if_not_exists(".build")) return 1;

    nob_cc(&cmd);
    cmd_append(&cmd, "-c");
    nob_cc_inputs(&cmd, "src/main.c");
    nob_cc_output(&cmd, ".build/main.o");
    nob_cc_flags(&cmd);
    cmd_append(&cmd, "-I./lib/");
    if (!nob_cmd_run(&cmd)) return 1;

    nob_cc(&cmd);
    nob_cc_inputs(&cmd, ".build/main.o");
    nob_cc_output(&cmd, "tech_support");
    cmd_append(&cmd, "-L./lib/", "-l:libraylib.a", "-lm");
    if (!nob_cmd_run(&cmd)) return 1;

    if (run) {
        cmd_append(&cmd, "./tech_support");
        if (!cmd_run(&cmd)) return 1;
    }

    return 0;
}
