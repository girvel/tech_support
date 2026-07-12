#pragma once
#include "ecs.h"

extern Entity game_player;

void display_init();
void display_update();
void display_register(const Entity *e);
void terminal_write(const char *str, Color color);
