#ifndef ENTITY_H
#define ENTITY_H

#include "ECScomponents.h"

typedef int EntityID;

struct registryView
{
    EntityID* view;
    int count;
};

void initECS();
void destroyECS();

EntityID newEntity();
unsigned int getEntityCount();

#define ECSaddComponent(id, type) ecs_add_component_internal((id), (type), sizeof(type##Component))
#define ECSgetComponent(id, type) ecs_get_component_internal((id), (type), sizeof(type##Component))

int hasComponent(EntityID id, enum ComponentType type);
void* ecs_add_component_internal(EntityID id, enum ComponentType type, unsigned int size);
void* ecs_get_component_internal(EntityID id, enum ComponentType type, unsigned int size);

struct registryView ECSviewRegistry(enum ComponentType type);
struct registryView ECSgroupView(enum ComponentType t1, enum ComponentType t2);
void closeView(struct registryView view);

#endif