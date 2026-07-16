#include "animated.h"
#include "components/sprite.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

char tmp_buf[4096];
char *tprintf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
char *tprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsnprintf(tmp_buf, size(tmp_buf), fmt, args);
    va_end(args);
    return tmp_buf;
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
    };

    Image img = LoadImage(tprintf("%s/idle_01.png", path));
    assert(img.height == cell_size && "Animation atlas height should be equal to cell_size");
    assert(img.width % cell_size == 0 && "Animation atlas width should be a multiple of cell_size");
    assert(img.width / cell_size == ROTATION_N && "Wrong number of sprites in animation atlas");

    const int count = 1;
    Animation idle = {
        .frames = malloc(sizeof(Sprite) * count * ROTATION_N),
        .count = count,
    };

    forn (rot, 0, ROTATION_N) {
        Image crop = ImageCopy(img);
        ImageCrop(&crop, (Rectangle) {rot * cell_size, 0, cell_size, cell_size});
        sprite_read(&idle.frames[0][rot], &crop);
        UnloadImage(crop);
    }
    UnloadImage(img);

    return result;
}

// TODO freeing animations
