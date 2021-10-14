#ifndef ECS_C_H
#define ECS_C_H

#include "c_transform.h"
#include "c_sprite.h"
#include "c_physics.h"

#define MAX_COMPONENT 3
enum ComponentType
{
    Transform = 0,
    Sprite,
    Physics,
};

#endif