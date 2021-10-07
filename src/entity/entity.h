#ifndef ENTITY_H
#define ENTITY_H

#include "ECScomponents.h"

typedef int EntityID;

typedef struct 
{
    vec3s color;
    vec3s pos;
    vec3s speed;
}Entity;

void initECS();
EntityID newEntity();

int hasComponent(EntityID id, enum ComponentType type);
void addComponent(EntityID id, enum ComponentType type, void* component);
void* getComponent(EntityID id, enum ComponentType type);

void* registerView(enum ComponentType type);

#endif