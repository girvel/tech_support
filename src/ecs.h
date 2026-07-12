#pragma once
#include "raylib.h"

typedef struct {
    Texture *texture;
    Vector2 *position;
} Entity;

void ecs_texture(Entity *e, Texture tex);
void ecs_position(Entity *e, float x, float y);
void ecs_positionv(Entity *e, Vector2 pos);
void ecs_add(Entity *e);
Entity *ecs_entities();
