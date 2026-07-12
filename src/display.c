#include "display.h"
#include "terminal.h"
#include <math.h>
#include <utils.h>
#include <stb_ds.h>

Mode display_mode = MODE_TERMINAL;

typedef struct {
    Vector2 *position;
    Texture *texture;
} Target;

static Target *targets = NULL;

static const int scale = 4;
static const int cell_size = 16;

Entity game_player = {0};

static void game_update()
{
    foreach (target, targets) {
        Vector2 pos_scaled = *target->position;
        pos_scaled.x *= scale * cell_size;
        pos_scaled.y *= scale * cell_size;
        DrawTextureEx(*target->texture, pos_scaled, 0, scale, WHITE);
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

void display_register(const Entity *e)
{
    if (e->texture == NULL || e->position == NULL) return;
    Target t = {
        .position = e->position,
        .texture = e->texture,
    };
    arrput(targets, t);
}
