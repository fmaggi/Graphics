#ifndef WORLD_H
#define WORLD_H

#include "entity/entity.h"

#define MAX_NTTs 16


typedef struct _world
{
    EntityID player;
    unsigned int count;
} World;

World* emptyWorld();

typedef void (*WorldFuncPtr)(World* w);

extern WorldFuncPtr initWorld;
extern WorldFuncPtr destroyWorld;

#endif