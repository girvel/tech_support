#pragma once
#include "components/sprite.h"
#include <stddef.h>

typedef enum {
    ROTATION_UP,
    ROTATION_LEFT,
    ROTATION_DOWN,
    ROTATION_RIGHT,
    #define ROTATION_N 4
} Rotation;

typedef enum {
    ANIMATION_IDLE,
    #define ANIMATION_N 1
} AnimationKind;

typedef struct {
    Sprite (*frames)[ROTATION_N];  // ptr to Sprite[ROTATION_N]
    size_t count;
} Animation;

typedef struct {
    Animation pack[ANIMATION_N];
    AnimationKind current, next;
    float frame;
    bool paused, looped;
} AnimationState;

AnimationState *animated(const char *path);
