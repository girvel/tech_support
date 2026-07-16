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

void sprite_read(Sprite *result, Image *img);

// Read a sprite as an atlas
Sprite *spritea(Image img, size_t *n);

// Read a sprite from the image whole; mutates the image
Sprite *spritei(Image *img);

extern const int cell_size;
extern const Color anchor_colors[ANCHOR_N];
