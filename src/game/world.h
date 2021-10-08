#ifndef WORLD_H
#define WORLD_H

#include "entity/entity.h"

typedef struct _world
{
    EntityID player;
} World;

extern World world;

void initWorld();
void destroyWorld();
void onUpdateWorld(double ts);

#endif