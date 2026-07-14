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

static inline void sprite_read(Sprite *result, Image *img)
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
}

Sprite *spritea(Image img, size_t *n)
{
    assert(img.height == cell_size && "Sprite height should be equal to cell_size");
    assert(img.width % cell_size == 0 && "Sprite width should be a multiple of cell_size");
    size_t images_n = img.width / cell_size;
    if (n != NULL) *n = images_n;

    Sprite *result = malloc(sizeof(Sprite) * images_n);

    forn (i, 0, images_n) {
        Image crop = ImageCopy(img);
        ImageCrop(&crop, (Rectangle) {cell_size * i, 0, cell_size, cell_size});
        sprite_read(result + i, &crop);
    }

    return result;
}

Sprite *spritei(Image *img)
{
    Sprite *result = malloc(sizeof(Sprite));
    sprite_read(result, img);
    return result;
}
