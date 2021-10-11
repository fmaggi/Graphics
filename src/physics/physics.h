#ifndef PHYSICS_H
#define PHYSICS_H

#define ACTIVE 0b1
#define INACTIVE 0
#define DYNAMIC 0b10
#define STATIC 0

#include "cglm/struct.h"

#include "util/types.h"

struct AABB
{
    vec2s min, max;
    int32t e;
};

struct PhysicsObject
{
    vec3s position, scale, speed, force;
    struct AABB aabb;
    int8t flags;
};

void initPhysics(int width, int height);
void setPhysicsWidthHeight(int width, int height);

void updatePhysics(double ts);

#endif