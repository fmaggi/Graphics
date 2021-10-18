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

#define ECSaddComponent(id, type) ecs_add_component_internal((id), (type##_E), sizeof(type))
#define ECSgetComponent(id, type) ecs_get_component_internal((id), (type##_E), sizeof(type))
#define ECShasComponent(id, type) has_component_internal((id), (type##_E))
#define ECSviewRegistry(type) ecs_view_egistry_internal((type##_E))
#define ECSgroupView(type1, type2) ecs_group_view_internal((type1##_E), (type2##_E))

int has_component_internal(EntityID id, enum ComponentType type);
void* ecs_add_component_internal(EntityID id, enum ComponentType type, unsigned int size);
void* ecs_get_component_internal(EntityID id, enum ComponentType type, unsigned int size);

struct registryView ecs_view_egistry_internal(enum ComponentType type);
struct registryView ecs_group_view_internal(enum ComponentType t1, enum ComponentType t2);
void closeView(struct registryView view);

#endif