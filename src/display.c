#include "display.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>
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
static Mode mode = MODE_GAME;

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
        const float speed = 3. * GetFrameTime();
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

typedef struct {
    Color color;
    char ch;
} Cell;

typedef struct {
    Cell *field;
    int w, h;
} Grid;

static Cell *grid_at(Grid *grid, int x, int y)
{
    if (x < 0 || y < 0 || x >= grid->w || y >= grid->h) return NULL;
    return grid->field + x + y * grid->w;
}

static int font_size = 24;
static Font jbmono;
static Grid terminal;
static int cursor_x = 0, cursor_y = 0;

static void terminal_update()
{
    Vector2 ch = MeasureTextEx(jbmono, "w", font_size, 1);
    for (int h = 0; h < terminal.h; h++) {
        for (int w = 0; w < terminal.w; w++) {
            Vector2 pos = {w * ch.x, h * ch.y};
            Cell *cell = grid_at(&terminal, w, h);
            char text[2] = {cell->ch, 0};
            DrawTextEx(jbmono, text, pos, font_size, 1, cell->color);
        }
    }
}

void terminal_write(const char *str, Color color)
{
    forchar (ch, str) {
        switch (*ch) {
        case '\n':
            cursor_y++;
            cursor_x = 0;
            break;
        default: {
            Cell *cell = grid_at(&terminal, cursor_x, cursor_y);
            cell->ch = *ch;
            cell->color = color;
            cursor_x++;
            if (cursor_x >= terminal.w) {
                cursor_x = 0;
                cursor_y++;
            }
        }
        }

        if (cursor_y >= terminal.h) {
            memmove(terminal.field,
                    terminal.field + terminal.w,
                    terminal.w * (terminal.h - 1) * sizeof(Cell));

            for (Cell *cell = terminal.field + terminal.w * (terminal.h - 1);
                 cell < terminal.field + terminal.w * terminal.h;
                 cell++) {
                cell->ch = ' ';
                cell->color = WHITE;
            }

            cursor_y--;
        }
    }
}

void display_init()
{
    jbmono = LoadFontEx("assets/fonts/jbmono.ttf", font_size, NULL, 0);

    terminal.w = 80;
    terminal.h = 25;
    terminal.field = malloc((terminal.w * terminal.h) * sizeof(Cell));
    for (Cell *cell = terminal.field; cell < terminal.field + terminal.w * terminal.h; cell++) {
        cell->ch = ' ';
        cell->color = BLACK;
    }
}

void display_update()
{
    ClearBackground(BLACK);

    switch (mode) {
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
