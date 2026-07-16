#pragma once
#include "components/animated.h"
#include "components/sprite.h"
#include "raylib.h"

typedef struct {
    Vector2 *position;
    Rotation *rotation;
    Sprite *sprite;
    AnimationState *animation_state;
} Entity;

Vector2 *position(float x, float y);
Vector2 *positionv(Vector2 pos);

void ecs_add(const Entity *e);
Entity *ecs_entities(size_t *size);
