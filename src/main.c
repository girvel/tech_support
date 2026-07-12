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
    SetTargetFPS(60);

    display_init();

    Entity player = {0};
    ecs_texture(&player, LoadTexture("assets/sprites/humanoid.png"));
    ecs_position(&player, 2, 3);
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
