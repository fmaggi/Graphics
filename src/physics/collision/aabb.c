#include "aabb.h"

typedef struct body Body;

typedef struct
{
    vec2s min, max;
    Body* body;
} AABB;

AABB aabbs[32];
int32t current = 0;

int32t createAABB(vec2s min, vec2s max, int32t body)
{
    AABB* aabb = &aabbs[current];
    aabb->body = body;
    aabb->min = min;
    aabb->max = max;

    return current++;
}