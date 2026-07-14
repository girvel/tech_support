#include "exit.h"
#include <systems/display.h>
#include <stdlib.h>

void *exit_init(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    return NULL;
}

ProgramStatus exit_update(void *payload)
{
    (void)payload;
    display_mode = MODE_GAME;
    return PROGRAM_EXIT;
}
