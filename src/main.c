#include <raylib.h>
#include <stb_ds.h>
#include <ecs.h>
#include <display.h>

// TODO:
// - movement
// - collisions
// - terminal emulator

int main()
{
    InitWindow(1280, 720, "Tech Support");

    display_init();

    terminal_write("Hello, world! Welcome to the terminal emulator\n\n", WHITE);

    for (int _ = 0; _ < 8; _++) {
        terminal_write("girvel", RED);
        terminal_write("@asgard % ", WHITE);
        terminal_write("echo ", GREEN);
        terminal_write("123\n123\n\n", WHITE);
    }

    Entity e = {0};
    ecs_texture(&e, LoadTexture("assets/sprites/humanoid.png"));
    ecs_position(&e, 2, 3);
    ecs_add(&e);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            display_update();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
