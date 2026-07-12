#include "terminal.h"
#include "utils.h"
#include <stb_ds.h>

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
static char *current_command = NULL;

static void prompt()
{
    terminal_write("> ", GREEN);
}

void terminal_init()
{
    jbmono = LoadFontEx("assets/fonts/jbmono.ttf", font_size, NULL, 0);

    terminal.w = 80;
    terminal.h = 25;
    terminal.field = malloc((terminal.w * terminal.h) * sizeof(Cell));
    for (Cell *cell = terminal.field; cell < terminal.field + terminal.w * terminal.h; cell++) {
        cell->ch = ' ';
        cell->color = BLACK;
    }

    terminal_write("Hello, world! Welcome to the terminal emulator\n\n", WHITE);
    prompt();
}

void terminal_update()
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

    if (IsKeyPressed(KEY_ENTER)) {
        arrput(current_command, '\0');
        terminal_write("\n", WHITE);

        char **argv = NULL;
        arrput(argv, current_command);

        bool was_space_before = true;
        foreach (ch, current_command) {
            bool is_space = *ch == ' ';
            if (is_space) {
                *ch = '\0';
                if (was_space_before) {
                    argv[arrlen(argv) - 1] = ch + 1;
                } else {
                    arrput(argv, ch + 1);
                }
            }
            was_space_before = is_space;
        }

        foreach (arg, argv) {
            terminal_write(*arg, WHITE);
            terminal_write("\n", WHITE);
        }

        terminal_write("\n", WHITE);
        current_command = NULL;  // TODO free argv
        prompt();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && arrlen(current_command) > 0) {
        arrpop(current_command);
        terminal_write("\b", WHITE);
    }

    int key;
    while ((key = GetCharPressed())) {
        if (key < 256) {
            char buf[2] = {key, 0};
            terminal_write(buf, WHITE);
            arrput(current_command, key);
        }
    }
}

void terminal_write(const char *str, Color color)
{
    forchar (ch, str) {
        Cell *cell = grid_at(&terminal, cursor_x, cursor_y);
        switch (*ch) {
        case '\n':
            cursor_y++;
            cursor_x = 0;
            break;
        case '\b':
            (cell - 1)->ch = ' ';
            cursor_x--;
            if (cursor_x < 0) {
                if (cursor_y > 0) {
                    cursor_x = terminal.w - 1;
                    cursor_y--;
                } else {
                    cursor_x = 0;
                }
            }
            break;
        default:
            cell->ch = *ch;
            cell->color = color;
            cursor_x++;
            if (cursor_x >= terminal.w) {
                cursor_x = 0;
                cursor_y++;
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

