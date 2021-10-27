#ifndef ENTITY_H
#define ENTITY_H

#include "ECScomponents.h"

typedef int EntityID;

struct View
{
    void* component;
    uint32_t count;
};

struct Group
{
    void *component1, *component2;
    uint32_t count;
    uint8_t mask;
};

struct GroupObject
{
    void *component1, *component2;
};

void initECS();
void destroyECS();

EntityID newEntity();
uint32_t getEntityCount();

#define ECSaddComponent(id, type) ecs_add_component_internal((id), (type##_E), sizeof(type))
#define ECSgetComponent(id, type) ecs_get_component_internal((id), (type##_E), sizeof(type))
#define ECShasComponent(id, type) has_component_internal((id), (type##_E))

#define ECSview(component) ecs_view_internal((component##_E))
#define ECSviewGet(view, component, i) ecs_view_get_internal((view), (sizeof(component)), (component##_E), (i))

#define ECSgroupView(type1, type2) ecs_group_view_internal((type1##_E), (type2##_E))
#define ECSgroupGet(group, i, component1, component2) ecs_group_get_internal((group), (sizeof(component1)), (sizeof(component2)), (i))

int has_component_internal(EntityID id, enum ComponentType type);
void* ecs_add_component_internal(EntityID id, enum ComponentType type, unsigned int size);
void* ecs_get_component_internal(EntityID id, enum ComponentType type, unsigned int size);

struct View ecs_view_internal(enum ComponentType type);
void* ecs_view_get_internal(struct View* view, uint32_t size, enum ComponentType type, uint32_t i);

struct Group ecs_group_view_internal(enum ComponentType t1, enum ComponentType t2);
struct GroupObject ecs_group_get_internal(struct Group* group, uint32_t size1, uint32_t size2, uint32_t i);

#endif
