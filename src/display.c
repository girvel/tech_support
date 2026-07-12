#include "display.h"
#include <stb_ds.h>

typedef struct {
    Vector2 *position;
    Texture *texture;
} Target;

static Target *targets = NULL;
static const int scale = 4;
static const int cell_size = 16;

void display_update()
{
    foreach (target, targets) {
        Vector2 pos_scaled = *target->position;
        pos_scaled.x *= scale * cell_size;
        pos_scaled.y *= scale * cell_size;
        DrawTextureEx(*target->texture, pos_scaled, 0, scale, WHITE);
    }
}

void display_register(const Entity *e)
{
    if (e->texture == NULL || e->position == NULL) return;
    Target t = {
        .position = e->position,
        .texture = e->texture,
    };
    arrput(targets, t);
}
