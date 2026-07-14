#include <raylib.h>
#include <stb_ds.h>
#include <ecs.h>
#include <systems/display.h>

// TODO:
// - movement
// - collisions
// - terminal emulator

int main()
{
    InitWindow(1280, 720, "Tech Support");
    SetTargetFPS(60);

    display_init();

    Entity player = {0};
    texture(&player, LoadTexture("assets/sprites/humanoid.png"));
    position(&player, 2, 3);
    ecs_add(&player);
    game_player = player;

    while (!WindowShouldClose()) {
        BeginDrawing();
            display_update();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
