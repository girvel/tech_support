#pragma once
#include "raylib.h"
#include <stddef.h>

typedef enum {
    ANCHOR_PARENT,
    ANCHOR_HAND,
    ANCHOR_OFFHAND,
    ANCHOR_HEAD,
    ANCHOR_RIGHT_POCKET,
    #define ANCHOR_N 5
} SpriteAnchor;

typedef struct {
    Texture texture;
    Vector2 anchors[ANCHOR_N];
} Sprite;

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
    bool paused, looped, synced;
} AnimationState;

typedef struct {
    Vector2 *position;
    Rotation *rotation;
    Sprite *sprite;
    AnimationState *animation_state;
} Entity;

Vector2 *position(float x, float y);
Vector2 *positionv(Vector2 pos);

Rotation *rotation(Rotation rot);

void ecs_add(const Entity *e);
Entity *ecs_entities(size_t *size);

void sprite_read(Sprite *result, Image *img);

void rotate(Rotation *e_rotation, AnimationState *e_ast, Rotation rotation);

// Read a sprite as an atlas
Sprite *spritea(Image img, size_t *n);

// Read a sprite from the image whole; mutates the image
Sprite *spritei(Image *img);

extern const int sprite_size;
extern const Color sprite_anchors[ANCHOR_N];

AnimationState *animated(const char *path);
