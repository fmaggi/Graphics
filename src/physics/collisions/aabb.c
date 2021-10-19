#include "aabb.h"

#include "log/log.h"

typedef struct body Body;

typedef struct
{
    vec2s min, max, radius;
    Body* body;
} AABB;

AABB aabbs[32];
static int current = 0;

int sorted[32] = {0};

void sort()
{
    for (int i = 1; i < current; i++)
    {
        int index = sorted[i];
        int j = i-1;
        int jndex = sorted[j];
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
    int active[32] = {-1};
    int index = 0;
    for (int i = 0; i < current; i++)
    {
        active[index] = sorted[i];
        for (int j = 0; j < index; j++) // check from right to left if it intersects anything in the interval.
        {
            if (active[j] == -1)
                continue;
            AABB b = aabbs[active[index]];
            AABB a = aabbs[active[j]];
            if (b.min.x < a.max.x)
            {
                struct Collision* c = &results->collisions[results->count++];
                c->left = a.body;
                c->right = b.body;
                c->minSepareation.x = fabs(a.radius.x + b.radius.x);
                c->minSepareation.y = fabs(a.radius.y + b.radius.y);
            }
            else
            {
                active[j] = -1;
            }
        }
        index++;
    }
}

int testOverlap(int aID, int bID)
{
    AABB a = aabbs[aID];
    AABB b = aabbs[bID];
    return a.min.x < b.max.x && a.max.x > b.min.x
        && a.min.y < b.max.y && a.max.y > b.min.y;
}

// collideAABB(int aID, int bID, vec2s centerDistance, struct ContactPoint* cp)
// {
//     AABB a = aabbs[aID];
//     AABB b = aabbs[bID];

//     vec2s separation = (vec2s){{
//         (a.max.x - a.radius.x) - (b.max.x - b.radius.x),
//         (a.max.y - a.radius.y) - (b.max.y - b.radius.y),
//     }};

//     cp->minSeparation.x = fabs(a.radius.x + b.radius.x);
//     cp->minSeparation.y = fabs(a.radius.y + b.radius.y);

//     cp->normal = (vec2s){
//         {separation.x > separation.y, separation.x < separation.y}
//     };
// }

int createAABB2(vec2s center, vec2s halfExtents, void* body)
{
    AABB* aabb = &aabbs[current];
    aabb->body = body;
    
    aabb->min.x = center.x - halfExtents.x;
    aabb->min.y = center.y - halfExtents.y;
    aabb->max.x = center.x + halfExtents.x;
    aabb->max.y = center.y + halfExtents.y;
    aabb->radius = halfExtents;

    sorted[current] = current;
    return current++;
}

void updateAABB(int id, vec3s position)
{
    AABB* a = &aabbs[id];

    float hWidth = a->radius.x;
    float hHeihgt = a->radius.y;

    a->min = (vec2s){{position.x - hWidth, position.y - hHeihgt}};
    a->max = (vec2s){{position.x + hWidth, position.y + hHeihgt}};
}