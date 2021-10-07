#ifndef WORLD_H
#define WORLD_H

#include "entity/entity.h"

#define MAX_NTTs 16


typedef struct _world
{
    Entity player;
    unsigned int index;
} World;

World* emptyWorld();

typedef void (*WorldFuncPtr)(World* w);

extern WorldFuncPtr initWorld;
extern WorldFuncPtr destroyWorld;

#endif