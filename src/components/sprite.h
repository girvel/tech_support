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

// Mutates img
Sprite *spritei(Image *img);

extern const int cell_size;
extern const Color anchor_colors[ANCHOR_N];
