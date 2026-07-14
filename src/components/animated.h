#pragma once
#include "raylib.h"
#include <stddef.h>

typedef enum {
    ROTATION_UP,
    ROTATION_LEFT,
    ROTATION_DOWN,
    ROTATION_RIGHT,
} Rotation;

typedef enum {
    ANIMATION_IDLE,
} AnimationKind;

typedef struct {
    Texture *frames;
    size_t count;
} Animation;

typedef struct {
    Animation (*pack)[4];
    AnimationKind current, next;
    int frame;
    bool paused, looped;
} Animated;

void animated(const char *path);
