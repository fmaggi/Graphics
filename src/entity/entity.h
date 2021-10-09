#ifndef ENTITY_H
#define ENTITY_H

#include "ECScomponents.h"

typedef int EntityID;

void initECS();
void destroyECS();

EntityID newEntity();
unsigned int getEntityCount();

#define ECSaddComponent(id, type, component) ecs_add_component_internal((id), sizeof(type##Component), (type), (&component))

#define ECSgetComponent(id, type) ecs_get_component_internal((id), (type), sizeof(type##Component))

void ecs_add_component_internal(EntityID id, unsigned int size, enum ComponentType type, void* component);
void* ecs_get_component_internal(EntityID id, enum ComponentType type, unsigned int size);

void* registerView(enum ComponentType type);

#endif