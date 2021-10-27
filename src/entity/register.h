#ifndef REGISTER_H
#define REGISTER_H

#include "ECScomponents.h"

typedef unsigned char ComponentsUsed; // just a bit map to mark used components by entities. with unsigned char = 8 components per entity

struct Component
{
    void* components;
    uint32_t count;
};

struct Register
{
    ComponentsUsed* used;
    struct Component Components[MAX_COMPONENT];
};

extern struct Register registers;

#endif
