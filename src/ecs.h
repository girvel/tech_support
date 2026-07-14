#pragma once
#include "animated.h"
#include "raylib.h"

typedef struct {
    Vector2 *position;
    Texture *texture;
    Animated *animation;
} Entity;

void texture(Entity *e, Texture tex);
void position(Entity *e, float x, float y);
void positionv(Entity *e, Vector2 pos);

void ecs_add(const Entity *e);
Entity *ecs_entities(size_t *size);
