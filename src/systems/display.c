#include "display.h"
#include "terminal.h"
#include <math.h>
#include <utils.h>
#include <stb_ds.h>

Mode display_mode = MODE_GAME;

typedef struct {
    Vector2 *position;
    Sprite *sprite;
} Target;

static Target *targets = NULL;

static const int scale = 4;

Entity game_player = {0};

static void game_update()
{
    foreach (target, targets) {
        Vector2 pos_scaled = *target->position;
        pos_scaled.x *= scale * sprite_size;
        pos_scaled.y *= scale * sprite_size;
        DrawTextureEx(target->sprite->texture, pos_scaled, 0, scale, WHITE);
    }

    if (game_player.position) {
        const float speed = 5. * GetFrameTime();
        Vector2 delta = {
            (IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * speed,
            (IsKeyDown(KEY_S) - IsKeyDown(KEY_W)) * speed,
        };

        if (delta.x != 0 && delta.y != 0) {
            delta.x /= sqrt(2);
            delta.y /= sqrt(2);
        }

        if (delta.x != 0 || delta.y != 0) {
            if (game_player.rotation && game_player.animation_state) {
                Rotation rot =
                    IsKeyDown(KEY_W) ? ROTATION_UP :
                    IsKeyDown(KEY_S) ? ROTATION_DOWN :
                    IsKeyDown(KEY_A) ? ROTATION_LEFT :
                    ROTATION_RIGHT;
                rotate(game_player.rotation, game_player.animation_state, rot);
            }
        }

        game_player.position->x += delta.x;
        game_player.position->y += delta.y;
    }
}

void display_init()
{
    terminal_init();
}

void display_update()
{
    ClearBackground(BLACK);

    switch (display_mode) {
    case MODE_GAME: return game_update();
    case MODE_TERMINAL: return terminal_update();
    }

    DrawFPS(100, 100);
}

void display_register(const Entity *e, size_t id)
{
    if (e->sprite == NULL || e->position == NULL) return;
    Target t = {
        .position = e->position,
        .sprite = e->sprite,
    };
    arrput(targets, t);
}
