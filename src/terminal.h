#pragma once
#include "raylib.h"

void terminal_init(void);
void terminal_update(void);
void terminal_write(const char *str, Color color);
char terminal_read(Color color);
bool terminal_execute(int argc, char **argv);
