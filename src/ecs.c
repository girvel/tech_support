#include "ecs.h"
#include <assert.h>
#include <stdlib.h>
#include <stb_ds.h>

static Entity *all_entities = NULL;

void ecs_texture(Entity *e, Texture tex)
{
    assert(e->texture == NULL);
    e->texture = malloc(sizeof(Texture));
    *e->texture = tex;
}

void ecs_position(Entity *e, float x, float y)
{
    Vector2 pos = {x, y};
    ecs_positionv(e, pos);
}

void ecs_positionv(Entity *e, Vector2 pos)
{
    assert(e->position == NULL);
    e->position = malloc(sizeof(Vector2));
    *e->position = pos;
}

void ecs_add(Entity *e)
{
    arrput(all_entities, *e);
    *e = (Entity) {0};
}

Entity *ecs_entities()
{
    return all_entities;
}
