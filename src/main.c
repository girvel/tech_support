#include "systems/animation.h"
#include <raylib.h>
#include <stb_ds.h>
#include "ecs.h"
#include "systems/display.h"

// TODO:
// - movement
// - collisions
// - terminal emulator

int main()
{
    InitWindow(1280, 720, "Tech Support");
    SetTargetFPS(60);

    display_init();

    Image humanoid = LoadImage("assets/sprites/humanoid.png");
    Entity player = {
        .animation_state = animated("assets/animations/humanoid"),
        .sprite = spritea(humanoid, NULL) + 1,  // TODO empty sprite
        .position = position(2, 3),
        .rotation = rotation(ROTATION_DOWN),
    };
    UnloadImage(humanoid);
    ecs_add(&player);
    game_player = player;

    while (!WindowShouldClose()) {
        BeginDrawing();
            display_update();
            animation_update();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
