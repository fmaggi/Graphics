#ifndef AABB_H
#define AABB_H

#include "cglm/struct.h"
#include "util/types.h"

struct Collision
{
    void *a, *b;
};

struct CollisionStack
{
    struct Collision* collisions;
    int32t count;
};

int32t createAABB2(vec2s min, vec2s max, void* bodyID);


void sweepAndPrune(struct CollisionStack* results);

int collide(int32t aID, int32t bID);

#endif