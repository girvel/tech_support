#include "terminal.h"
#include "display.h"
#include "programs/exit.h"
#include "programs/shell.h"
#include <stdio.h>
#include <utils.h>
#include <stb_ds.h>
#include <programs/sleep.h>
#include <program.h>

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

static Program programs[] = {  // TODO hashmap
    {.name = "shell", .init = shell_init, .update = shell_update},
    {.name = "sleep", .init = sleep_init, .update = sleep_update},
    {.name = "exit", .init = exit_init, .update = exit_update},
};

typedef struct {
    Program pr;
    void *payload;
    char **argv; // that is an stb array
    char *argv_data;
} Process;

static Process *process_stack = NULL;

bool terminal_execute(int argc, char **argv)
{
    size_t total_len = 0;
    for (char **arg = argv; arg < argv + argc; arg++) {
        total_len += strlen(*arg) + 1;
    }

    char **argv_copy = NULL;  // TODO terminal_arena + stb_hs
    char *argv_copy_data = malloc(total_len);  // TODO terminal_arena

    size_t i = 0;
    for (char **arg = argv; arg < argv + argc; arg++) {
        arrput(argv_copy, argv_copy_data + i);
        strcpy(argv_copy_data + i, *arg);
        i += strlen(*arg) + 1;
    }

    forarr (pr, programs) {
        if (strcmp(pr->name, argv[0]) == 0) {
            Process pc = {
                .pr = *pr,
                .payload = pr->init(argc, argv_copy),
                .argv = argv_copy,
                .argv_data = argv_copy_data,
            };
            arrput(process_stack, pc);
            return true;
        }
    }

    return false;
}

void terminal_init()
{
    jbmono = LoadFontEx("assets/fonts/jbmono.ttf", font_size, NULL, 0);

    terminal.w = 80;
    terminal.h = 25;
    terminal.field = malloc((terminal.w * terminal.h) * sizeof(Cell));  // TODO global_arena
    for (Cell *cell = terminal.field; cell < terminal.field + terminal.w * terminal.h; cell++) {
        cell->ch = ' ';
        cell->color = BLACK;
    }

    char *argv[] = {"shell"};
    terminal_execute(size(argv), argv);
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

    Process current_pc = process_stack[arrlen(process_stack) - 1];
    ProgramStatus status = current_pc.pr.update(current_pc.payload);
    if (status == PROGRAM_EXIT) {
        Process pc = arrpop(process_stack);
        arrfree(pc.argv);
        free(pc.argv_data);
        free(pc.payload);  // TODO _deinit to free used resources + program_default_deinit
        if (arrlen(process_stack) == 0) display_mode = MODE_GAME;
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

