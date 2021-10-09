#ifndef ECS_C_H
#define ECS_C_H

#include "c_transform.h"
#include "c_sprite.h"
#include "c_physics.h"
#include "c_script.h"

#define MAX_COMPONENT 4
enum ComponentType
{
    Transform = 0,
    Sprite,
    Physics,
    Script
};

#endif