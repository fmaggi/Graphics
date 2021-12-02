#ifndef ECS_C_H
#define ECS_C_H


#include "glm/glm.hpp"

typedef uint32_t EntityID;

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
