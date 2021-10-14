#include "aabb.h"

#include "log/log.h"

typedef struct body Body;

typedef struct
{
    vec2s min, max;
    Body* body;
} AABB;

AABB aabbs[32];
static int32t current = 0;

int32t sorted[32] = {0};

void sort()
{
    for (int32t i = 1; i < current; i++)
    {
        int32t index = sorted[i];
        int32t j = i-1;
        int32t jndex = sorted[j];
        float min = aabbs[index].min.x;

        while (j >= 0 && aabbs[jndex].min.x > min)
        {
            sorted[j+1] = jndex;
            --j;
            jndex = sorted[j];
        }
        sorted[j+1] = index;
    }
}

void sweepAndPrune(struct CollisionStack* results)
{
    sort();
    AABB active[32];
    int index = 0;
    for (int i = 0; i < current; i++)
    {
        active[index++] = aabbs[sorted[i]];
        for (int j = index-2; j >= 0; j--) // check from right to left if it intersects anything in the interval.
        {
            AABB b = active[index-1];
            AABB a = active[j];
            if (b.min.x < a.max.x)
            {
                struct Collision* c = &results->collisions[results->count++];
                c->left = a.body;
                c->right = b.body;
            }
            else
            {
                active[0] = b;
                index = 1;
                break;
            }
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
    sorted[current] = current;
    return current++;
}

void updateAABB(int32t id, vec3s position)
{
    AABB* a = &aabbs[id];
    float hWidth = (a->max.x - a->min.x) / 2;
    float hHeihgt = (a->max.y - a->min.y) / 2;

    a->min = (vec2s){position.x - hWidth, position.y - hHeihgt};
    a->max = (vec2s){position.x + hWidth, position.y + hHeihgt};
}