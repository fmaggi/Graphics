#ifndef ECS_C_H
#define ECS_C_H

#include "c_transform.h"
#include "c_sprite.h"

#define MAX_COMPONENT 2
enum ComponentType
{
    Transform = 0,
    Sprite,
};

#endif