#include "display.h"
#include <stb_ds.h>

typedef struct {
    Vector2 *position;
    Texture *texture;
} Target;

typedef enum {
    MODE_GAME,
    MODE_TERMINAL,
} Mode;

static Target *targets = NULL;
static Mode mode = MODE_TERMINAL;

static const int scale = 4;
static const int cell_size = 16;

static void game_update()
{
    foreach (target, targets) {
        Vector2 pos_scaled = *target->position;
        pos_scaled.x *= scale * cell_size;
        pos_scaled.y *= scale * cell_size;
        DrawTextureEx(*target->texture, pos_scaled, 0, scale, WHITE);
    }
}

typedef struct {
    char *field;
    int w, h;
} Grid;

static char *grid_at(Grid *grid, int x, int y)
{
    return grid->field + x + y * grid->w;
}

static int font_size = 24;
static Font jbmono;
static Grid terminal;

static void terminal_update()
{
    Vector2 ch = MeasureTextEx(jbmono, "w", font_size, 1);
    for (int h = 0; h < terminal.h; h++) {
        for (int w = 0; w < terminal.w; w++) {
            Vector2 pos = {w * ch.x, h * ch.y};
            char text[2] = {*grid_at(&terminal, w, h), 0};
            DrawTextEx(jbmono, text, pos, font_size, 1, WHITE);
        }
    }
}

void display_init()
{
    terminal.w = 80;
    terminal.h = 25;
    terminal.field = malloc((terminal.w * terminal.h) * sizeof(char));
    jbmono = LoadFontEx("assets/fonts/jbmono.ttf", font_size, NULL, 0);

    for (int h = 0; h < terminal.h; h++) {
        for (int w = 0; w < terminal.w; w++) {
            *grid_at(&terminal, w, h) = ' ';
        }
    }
    *grid_at(&terminal, 1, 0) = '>';
}

void display_update()
{
    switch (mode) {
    case MODE_GAME: return game_update();
    case MODE_TERMINAL: return terminal_update();
    }
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
