#ifndef C_PHYSICS_H
#define C_PHYSICS_H

#include "cglm/struct.h"

enum BoundingBox
{
    sphere = 0,
    AABB = 1,
};

typedef struct
{
    vec2s force;
    enum BoundingBox bb;
    int dynamic;
    int active;
    int SpaceMask;
} PhysicsComponent;

#endif