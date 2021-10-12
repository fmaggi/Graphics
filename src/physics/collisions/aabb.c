#include "aabb.h"

typedef struct body Body;

typedef struct
{
    vec2s min, max;
    Body* body;
} AABB;

AABB aabbs[32];
static int32t current = 0;

int32t sorted[32];

void sort()
{
    for (int32t i = 1; i < current; i++)
    {
        int32t j = i-1;
        float min = aabbs[i].min.x;

        while (j >= 0 && aabbs[i].min.x > min)
        {
            sorted[j+1] = j;
            --j;
        }
        sorted[j+1] = i;
    }
}

void sweepAndPrune(struct CollisionStack* results)
{
    sort();
    AABB active = aabbs[sorted[0]];
    for (int i = 1; i < current; i++)
    {
        AABB aabb = aabbs[sorted[1]];
        if (aabb.min.x < active.max.x)
        {
            struct Collision* c = &results->collisions[results->count++];
            c->a = active.body;
            c->b = aabb.body;
        }
        else
        {
            active = aabb;
        }
    }
}

int collide(int32t aID, int32t bID)
{
    AABB a = aabbs[aID];
    AABB b = aabbs[bID];
    return a.min.x < b.max.x && a.max.x > b.min.x
        && a.min.y < b.max.y && a.max.y > b.min.y;
}

int32t createAABB2(vec2s min, vec2s max, void* body)
{
    AABB* aabb = &aabbs[current];
    aabb->body = body;
    aabb->min = min;
    aabb->max = max;

    return current++;
}