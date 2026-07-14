#include "ecs.h"
#include <systems/display.h>
#include <assert.h>
#include <stdlib.h>
#include <stb_ds.h>

static Entity *all_entities = NULL;

Vector2 *position(float x, float y)
{
    Vector2 *result = malloc(sizeof(Vector2));  // TODO fixed-size allocator
    *result = (Vector2) {x, y};
    return result;
}

Vector2 *positionv(Vector2 pos)
{
    Vector2 *result = malloc(sizeof(Vector2));
    *result = pos;
    return result;
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
