#ifndef ENTITY_H
#define ENTITY_H

#include "ECScomponents.h"

typedef int EntityID;

void initECS();
void destroyECS();

EntityID newEntity();
unsigned int getEntityCount();

#define ECSaddComponent(id, type) ecs_add_component_internal((id), (type), sizeof(type##Component))
#define ECSgetComponent(id, type) ecs_get_component_internal((id), (type), sizeof(type##Component))

int hasComponent(EntityID id, enum ComponentType type);
void* ecs_add_component_internal(EntityID id, enum ComponentType type, unsigned int size);
void* ecs_get_component_internal(EntityID id, enum ComponentType type, unsigned int size);

void* registerView(enum ComponentType type);

#endif