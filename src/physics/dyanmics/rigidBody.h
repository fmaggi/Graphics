#ifndef BODY_H
#define BODY_H

#include "glm/glm.hpp"

enum BodyType
{
    Static = 0, Dynamic
};

typedef struct body
{
    glm::vec3 position;
    glm::vec2 speed, impulse;

    enum BodyType type;
    int32_t aabbID;

    void (*onCollision)(struct body* self, struct body* other);
    void* userData;
    uint32_t userFlags;
} Body;

typedef void (*CollisionCallback)(Body* self, Body* other);

void addAABB(Body* body, float halfWidth, float halfHeight);

#endif
