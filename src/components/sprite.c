#include "sprite.h"
#include "raylib.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const int cell_size = 16;
const Color anchor_colors[ANCHOR_N] = {
    [ANCHOR_PARENT] = rgb(0xff0000),
    [ANCHOR_HAND] = rgb(0xfb0000),
    [ANCHOR_OFFHAND] = rgb(0xf70000),
    [ANCHOR_HEAD] = rgb(0xf30000),
    [ANCHOR_RIGHT_POCKET] = rgb(0xef0000),
};

static void print_color(Color c)
{
    printf("%d %d %d %d\n", c.r, c.g, c.b, c.a);
}

Sprite *spritei(Image *img)
{
    // assert(img->height == cell_size && "Sprite height should be equal to cell_size");
    // assert(img->width % cell_size == 0 && "Sprite width should be a multiple of cell_size");
    // size_t images_n = img->width / cell_size;
    // if (n != NULL) *n = images_n;

    Sprite *result = malloc(sizeof(Sprite));
    for (size_t i = 0; i < ANCHOR_N; i++) result->anchors[i].x = -1;

    if (img->format != PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) {
        ImageFormat(img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    }

    Color *pixels = img->data;
    Color main_color = {0};
    for (int i = 0; i < img->width * img->height; i++) {
        Color pixel = pixels[i];
        if ((pixel.g == 0 && pixel.b == 0) || pixel.a == 0) continue;
        main_color = pixel;
        break;
    }

    for (int x = 0; x < img->width; x++) {
        for (int y = 0; y < img->height; y++) {
            Color *pixel = &pixels[y * img->width + x];
            if (pixel->g != 0 || pixel->b != 0 || pixel->a != 255) continue;
            for (int i = 0; i < ANCHOR_N; i++) {
                if (pixel->r == anchor_colors[i].r) {
                    assert(result->anchors[i].x == -1 && "Mulitple anchors of the same type");
                    result->anchors[i] = (Vector2) {x, y};
                    *pixel = main_color;
                    break;
                }
            }
        }
    }

    result->texture = LoadTextureFromImage(*img);
    return result;
}
