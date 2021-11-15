#ifndef REGISTER_H
#define REGISTER_H

#include <vector>

#include "ECScomponents.h"

typedef unsigned char ComponentsUsed; // just a bit map to mark used components by entities. with unsigned char = 8 components per entity
typedef int EntityID;

template<typename T>
struct Component
{
    T* components;
    EntityID* sparse;
    uint32_t count;
};

struct Register
{
    ComponentsUsed* used;
};

extern struct Register registers;

#endif
