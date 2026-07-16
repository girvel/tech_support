#include "ecs.h"
#include "systems/animation.h"
#include "utils.h"
#include <stdio.h>
#include <systems/display.h>
#include <assert.h>
#include <stdlib.h>
#include <stb_ds.h>

static Entity *all_entities = NULL;

void ecs_add(const Entity *e)
{
    size_t id = arrlen(all_entities);
    arrput(all_entities, *e);
    display_register(e, id);
    animation_register(e, id);
}

const Entity *ecs_get(size_t id)
{
    return &all_entities[id];
}

Entity *ecs_entities(size_t *size)
{
    *size = arrlen(all_entities);
    return all_entities;
}

Vector2 *position(float x, float y)
{
    Vector2 *result = malloc(sizeof(Vector2));  // TODO fixed-size allocator
    *result = (Vector2) {x, y};
    return result;
}

Vector2 *positionv(Vector2 pos)
{
    Vector2 *result = malloc(sizeof(Vector2));
    *result = pos;
    return result;
}

Rotation *rotation(Rotation rot)
{
    Rotation *result = malloc(sizeof(Rotation));
    *result = rot;
    return result;
}

const int sprite_size = 16;
const Color sprite_anchors[ANCHOR_N] = {
    [ANCHOR_PARENT] = rgb(0xff0000),
    [ANCHOR_HAND] = rgb(0xfb0000),
    [ANCHOR_OFFHAND] = rgb(0xf70000),
    [ANCHOR_HEAD] = rgb(0xf30000),
    [ANCHOR_RIGHT_POCKET] = rgb(0xef0000),
};

void sprite_read(Sprite *result, Image *img)
{
    forarr (anchor, result->anchors) anchor->x = -1;

    if (img->format != PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) {
        ImageFormat(img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    }

    Color *pixels = img->data;
    Color main_color = {0};
    forn (x, 0, img->width) {
        forn (y, 0, img->height) {
            Color pixel = pixels[y * img->width + x];
            if ((pixel.g == 0 && pixel.b == 0) || pixel.a == 0) continue;
            main_color = pixel;
            goto break_out;
        }
    }

break_out:
    forn (x, 0, img->width) {
        forn (y, 0, img->height) {
            Color *pixel = &pixels[y * img->width + x];
            if (pixel->g != 0 || pixel->b != 0 || pixel->a != 255) continue;
            for (int i = 0; i < ANCHOR_N; i++) {
                if (pixel->r == sprite_anchors[i].r) {
                    assert(result->anchors[i].x == -1 && "Mulitple anchors of the same type");
                    result->anchors[i] = (Vector2) {x, y};
                    *pixel = main_color;
                    break;
                }
            }
        }
    }

    result->texture = LoadTextureFromImage(*img);
}

Sprite *spritea(Image img, size_t *n)
{
    assert(img.height == sprite_size && "Sprite height should be equal to sprite_size");
    assert(img.width % sprite_size == 0 && "Sprite width should be a multiple of sprite_size");
    size_t images_n = img.width / sprite_size;
    if (n != NULL) *n = images_n;

    Sprite *result = malloc(sizeof(Sprite) * images_n);

    forn (i, 0, images_n) {
        Image crop = ImageCopy(img);
        ImageCrop(&crop, (Rectangle) {sprite_size * i, 0, sprite_size, sprite_size});
        sprite_read(result + i, &crop);
        UnloadImage(crop);
    }

    return result;
}

Sprite *spritei(Image *img)
{
    Sprite *result = malloc(sizeof(Sprite));
    sprite_read(result, img);
    return result;
}

void rotate(Rotation *e_rotation, AnimationState *e_ast, Rotation rotation)
{
    Rotation prev = *e_rotation;
    memcpy(e_rotation, &rotation, sizeof(Rotation));
    if (prev != rotation) {
        e_ast->synced = false;
    }
}

void animate(Entity *e, AnimationKind kind)
{
    // e->
}

AnimationState *animated(const char *path)  // TODO memoizing packs, but not sprites?
{
    AnimationState *result = malloc(sizeof(AnimationState));
    *result = (AnimationState) {
        .current = ANIMATION_IDLE,
        .next = ANIMATION_IDLE,
        .frame = 0,
        .paused = false,
        .looped = false,
        .synced = false,
    };

    Image img = LoadImage(tprintf("%s/idle_01.png", path));
    assert(img.height == sprite_size && "Animation atlas height should be equal to sprite_size");
    assert(img.width % sprite_size == 0 && "Animation atlas width should be a multiple of sprite_size");
    assert(img.width / sprite_size == ROTATION_N && "Wrong number of sprites in animation atlas");

    const int count = 1;
    
    result->pack[ANIMATION_IDLE] = (Animation) {
        .frames = malloc(sizeof(Sprite) * count * ROTATION_N),
        .count = count,
    };

    forn (rot, 0, ROTATION_N) {
        Image crop = ImageCopy(img);
        ImageCrop(&crop, (Rectangle) {rot * sprite_size, 0, sprite_size, sprite_size});
        sprite_read(&result->pack[ANIMATION_IDLE].frames[0][rot], &crop);
        UnloadImage(crop);
    }
    UnloadImage(img);

    return result;
}

// TODO freeing animations
