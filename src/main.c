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
    InitWindow(400, 400, "Tech Support");

    display_init();

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
