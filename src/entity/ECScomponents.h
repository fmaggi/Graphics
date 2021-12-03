#ifndef ECS_C_H
#define ECS_C_H

#include "entity.h"

#include "glm/glm.hpp"

struct TransformComponent
{
    glm::vec3 translation;
    float rotation;
    glm::vec2 scale;
};

struct  SpriteComponent
{
    glm::vec4 color;
    float texIndex;
};

struct PhysicsComponent
{
    void* physicsBody;
};

#endif
