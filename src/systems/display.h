#pragma once
#include "ecs.h"

typedef enum {
    MODE_GAME,
    MODE_TERMINAL,
} Mode;

extern Mode display_mode;
extern Entity game_player;

void display_init();
void display_update();
void display_register(const Entity *e, size_t id);
