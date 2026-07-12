#pragma once
#include <program.h>

void *shell_init(int argc, char **argv);
ProgramStatus shell_update(void *payload);
