#ifndef WORLD_H
#define WORLD_H

#include "events/event.h"

void initWorld();
void onUpdateWorld(double ts);
void onRenderWorld();
int  onEventWorld(void* event, enum EventType type);
void destroyWorld();

#endif
