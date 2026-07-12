#define NOB_IMPLEMENTATION
#include "lib/nob.h"
#define FLAG_IMPLEMENTATION
#include "lib/flag.h"

Nob_Cmd cmd = {0};
Procs procs = {0};

struct {
    bool run, debug;
} cli = {0};

void read_cli(int argc, char **argv)
{
    flag_bool_var(&cli.run, "run", false, "Run the program after compilation.");
    flag_bool_var(&cli.debug, "debug", false, "Run the program with GDB after compilation.");

    if (!flag_parse(argc, argv)) {
        flag_print_error(stderr);
        exit(1);
    }

    if (cli.run && cli.debug) {
        fprintf(stderr, "Both -run and -debug provided\n");
        exit(1);
    }
}

typedef Da(const char *) Strings;

bool attempt_compilation(Nob_Walk_Entry entry)
{
    Strings *outputs = entry.data;

    String_View sv = sv_from_cstr(entry.path);
    if (!sv_ends_with_cstr(sv, ".c")) return true;
    sv_chop_prefix(&sv, sv_from_cstr("src/"));
    assert(sv_chop_suffix(&sv, sv_from_cstr(".c")) && "Source should end with \".c\"");

    String_Builder sb = {0};
    sb_appendf(&sb, ".build/"SV_Fmt".o", SV_Arg(sv));
    sb_append_null(&sb);
    char *obj_file = sb.items;
    da_append(outputs, obj_file);

    nob_cc(&cmd);
    nob_cc_inputs(&cmd, entry.path);
    nob_cc_output(&cmd, obj_file);
    cmd_append(&cmd, "-c");
    nob_cc_flags(&cmd);
    cmd_append(&cmd, "-I./lib/", "-I./src/");
    if (cli.debug) {
        cmd_append(&cmd, "-ggdb");
    }

    return cmd_run(&cmd, .async = &procs);
}

bool compile_schedule_stb_ds(Strings *outputs)
{
    const char *obj_file = ".build/stb_ds.o";
    da_append(outputs, obj_file);

    nob_cc(&cmd);
    cmd_append(&cmd, "-c", "-x", "c", "-DSTB_DS_IMPLEMENTATION");
    nob_cc_inputs(&cmd, "lib/stb_ds.h");
    nob_cc_output(&cmd, obj_file);
    nob_cc_flags(&cmd);
    if (cli.debug) {
        cmd_append(&cmd, "-ggdb");
    }

    return cmd_run(&cmd, .async = &procs);
}

bool link_files(Strings obj_files)
{
    nob_cc(&cmd);
    da_foreach(const char *, obj_file, &obj_files) {
        nob_cc_inputs(&cmd, *obj_file);
    }
    nob_cc_output(&cmd, "tech_support");
    cmd_append(&cmd, "-L./lib/", "-l:libraylib.a", "-lGL", "-lm", "-lpthread", "-ldl", "-lX11");
    return nob_cmd_run(&cmd);
}

int main(int argc, char **argv)
{
    GO_REBUILD_URSELF(argc, argv);
    read_cli(argc, argv);

    if (!mkdir_if_not_exists(".build")) return 1;

    Strings obj_files = {0};
    if (!walk_dir("src", &attempt_compilation, .data = &obj_files)) return 1;
    compile_schedule_stb_ds(&obj_files);
    if (!procs_flush(&procs)) return 1;
    
    if (!link_files(obj_files)) return 1;

    if (cli.run) {
        cmd_append(&cmd, "./tech_support");
        if (!cmd_run(&cmd)) return 1;
    } else if (cli.debug) {
        cmd_append(&cmd, "gdb", "-ex", "run", "--args", "./tech_support");
        if (!cmd_run(&cmd)) return 1;
    }

    return 0;
}
