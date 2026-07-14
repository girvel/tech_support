#include "ecs.h"
#include <systems/display.h>
#include <assert.h>
#include <stdlib.h>
#include <stb_ds.h>

static Entity *all_entities = NULL;

void texture(Entity *e, Texture tex)
{
    assert(e->texture == NULL);
    e->texture = malloc(sizeof(Texture));
    *e->texture = tex;
}

void position(Entity *e, float x, float y)
{
    Vector2 pos = {x, y};
    positionv(e, pos);
}

void positionv(Entity *e, Vector2 pos)
{
    assert(e->position == NULL);
    e->position = malloc(sizeof(Vector2));
    *e->position = pos;
}

void ecs_add(const Entity *e)
{
    arrput(all_entities, *e);
    display_register(e);
}

Entity *ecs_entities(size_t *size)
{
    *size = arrlen(all_entities);
    return all_entities;
}
