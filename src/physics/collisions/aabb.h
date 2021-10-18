#ifndef AABB_H
#define AABB_H

#include "cglm/struct.h"

struct Collision
{
    void *left, *right;
    vec2s minSepareation;
};

struct CollisionStack
{
    struct Collision* collisions;
    int count;
};

int createAABB2(vec2s min, vec2s max, void* bodyID);
void updateAABB(int id, vec3s position);

void sweepAndPrune(struct CollisionStack* results);

int testOverlap(int aID, int bID);

#endif