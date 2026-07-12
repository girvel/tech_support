#include <raylib.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#define foreach(VARNAME, COLLECTION) \
    for (__typeof__(COLLECTION) VARNAME = COLLECTION; \
         VARNAME < COLLECTION + arrlen(COLLECTION); \
         VARNAME++)

// TODO:
// - ecs.c, ecs_add, ecs_texture, ecs_position
// - foreach -> stb_ds
// - display.c, two modes, scaling

typedef struct {
    Texture *texture;
    Vector2 *position;
} Entity;

static Entity *all_entities = NULL;

int main()
{
    InitWindow(400, 400, "Tech Support");

    Entity e = {
        .texture = malloc(sizeof(Texture)),
        .position = malloc(sizeof(Vector2)),
    };
    *e.texture = LoadTexture("assets/sprites/humanoid.png");
    e.position->x = 2;
    e.position->y = 3;
    arrput(all_entities, e);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            foreach (e, all_entities) {
                if (e->position && e->texture) {
                    DrawTextureEx(*e->texture, *e->position, 0, 1, WHITE);
                }
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
