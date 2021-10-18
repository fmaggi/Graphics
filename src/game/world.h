#ifndef WORLD_H
#define WORLD_H

#include "entity/entity.h"
#include "events/event.h"

typedef struct world
{
    EntityID player;
} World;

extern World world;

void initWorld();
void destroyWorld();
bool onEventWorld(EventHolder* event);
void onUpdateWorld(double ts);

#endif