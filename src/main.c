#include <raylib.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#define foreach(VARNAME, COLLECTION) \
    for (__typeof__(COLLECTION) _foreach_collection = COLLECTION, VARNAME = _foreach_collection; \
         VARNAME < _foreach_collection + arrlen(_foreach_collection); \
         VARNAME++)
#include <ecs.h>

// TODO:
// - foreach -> stb_ds
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
