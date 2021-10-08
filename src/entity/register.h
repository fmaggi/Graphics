#ifndef REGISTER_H
#define REGISTER_H

#include "ECScomponents.h"

typedef unsigned char ComponentsUsed; // just a bit map to mark used components by entities. with unsigned char = 8 components per entity

struct Register
{
    ComponentsUsed* used;
    TransformComponent* transforms;
    SpriteComponent* sprites;
};

extern struct Register registers;

#endif