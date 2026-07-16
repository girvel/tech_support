#include "animation.h"
#include "raylib.h"
#include "utils.h"
#include <assert.h>
#include <stb_ds.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    size_t id;
    Rotation *rotation;
    Sprite *sprite;
    AnimationState *animation_state;
} Target;

static Target *targets = NULL;
const float animation_fps = 6;

void animation_update()
{
    foreach (target, targets) {
        AnimationState *ast = target->animation_state;
        size_t before = ast->frame;
        ast->frame += GetFrameTime() * animation_fps;
        size_t after = ast->frame;

        if (before != after || target->sprite->texture.id == 0 || !ast->synced) {
            ast->synced = true;
            Animation animation = ast->pack[ast->current];
            assert(animation.count > 0);
            if (after >= animation.count) after %= animation.count;
            memcpy(target->sprite, &animation.frames[after][*target->rotation], sizeof(Sprite));
        }
    }
}

void animation_register(const Entity *e, size_t id)
{
    if (e->rotation && e->sprite && e->animation_state) {
        Target t = {  // TODO try X-macro
            .id = id,
            .rotation = e->rotation,
            .sprite = e->sprite,
            .animation_state = e->animation_state,
        };
        arrput(targets, t);
    }
}
