#ifndef ECS_C_H
#define ECS_C_H

#define MAX_COMPONENT 3
enum ComponentType
{
    TransformComponent_E = 0,
    SpriteComponent_E,
    PhysicsComponent_E,
};

#include "cglm/struct.h"

typedef struct
{
    vec3s position;
    float rotation;
    vec2s scale;
} TransformComponent;

typedef struct
{
    vec3s color;
    float texIndex;
} SpriteComponent;

typedef struct
{
    void* physicsBody;
} PhysicsComponent;

#endif