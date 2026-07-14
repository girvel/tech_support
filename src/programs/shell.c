#include "shell.h"
#include <stdlib.h>
#include <terminal.h>
#include <stb_ds.h>
#include <utils.h>

typedef struct {
    char *current_command;
    bool executing;
} ExitState;

static void prompt()
{
    terminal_write("\n> ", GREEN);
}

void *shell_init(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    terminal_write("Hello, world! Welcome to the terminal emulator\n", WHITE);
    prompt();

    ExitState *payload = malloc(sizeof(ExitState));  // TODO terminal_arena
    payload->current_command = NULL;
    payload->executing = false;

    return payload;
}

ProgramStatus shell_update(void *payload)
{
    ExitState *state = payload;

    if (state->executing) {
        state->executing = false;
        prompt();
    }

    if (IsKeyPressed(KEY_ENTER)) {
        arrput(state->current_command, '\0');
        terminal_write("\n", WHITE);

        char **argv = NULL;
        arrput(argv, state->current_command);

        bool was_space_before = true;
        foreach (ch, state->current_command) {
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

        if (!terminal_execute(arrlen(argv), argv)) {
            terminal_write("Not found!\n", RED);
        }

        state->executing = true;
        arrsetlen(state->current_command, 0);
        arrfree(argv);
    }

    if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
        && arrlen(state->current_command) > 0
    ) {
        (void)arrpop(state->current_command);
        terminal_write("\b", WHITE);
    }

    int key;
    while ((key = GetCharPressed())) {
        if (key < 256) {
            char buf[2] = {key, 0};
            terminal_write(buf, WHITE);
            arrput(state->current_command, key);
        }
    }

    return PROGRAM_PROCEED;
}
