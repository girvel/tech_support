#include <raylib.h>
#include <stb_ds.h>
#include <ecs.h>

// TODO:
// - display.c, two modes, scaling

int main()
{
    InitWindow(400, 400, "Tech Support");

    Entity e = {0};
    ecs_texture(&e, LoadTexture("assets/sprites/humanoid.png"));
    ecs_position(&e, 2, 3);
    ecs_add(&e);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            foreach (e, ecs_entities()) {
                if (e->position && e->texture) {
                    DrawTextureEx(*e->texture, *e->position, 0, 1, WHITE);
                }
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
