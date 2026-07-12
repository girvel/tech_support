#pragma once
#include "program.h"

void *sleep_init(int argc, char **argv);
ProgramStatus sleep_update(void *payload);
