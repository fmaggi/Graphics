#ifndef ENTITY_H
#define ENTITY_H

#include "ECScomponents.h"

typedef int EntityID;

void initECS();
void destroyECS();

EntityID newEntity();
unsigned int getEntityCount();

int hasComponent(EntityID id, enum ComponentType type);
void addComponent(EntityID id, enum ComponentType type, void* component);
void* getComponent(EntityID id, enum ComponentType type);

void* registerView(enum ComponentType type);

#endif