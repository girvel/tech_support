#include "sleep.h"
#include <program.h>
#include <assert.h>
#include <stdlib.h>

void *sleep_init(int argc, char **argv)
{
    assert(argc == 2);
    int *frames = malloc(sizeof(int));
    *frames = atoi(argv[1]);
    return frames;
}

ProgramStatus sleep_update(void *payload)
{
    int *frames = payload;
    if (*frames <= 1) return PROGRAM_EXIT;
    (*frames)--;
    return PROGRAM_PROCEED;
}
