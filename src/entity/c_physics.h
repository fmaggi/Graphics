#ifndef C_PHYSICS_H
#define C_PHYSICS_H

#include "cglm/struct.h"

#define ACTIVE 0b1
#define INACTIVE 0
#define DYNAMIC 0b10
#define STATIC 0

typedef struct
{
    vec2s force;
    vec2s speed;
    char flags;  
} PhysicsComponent;

#endif