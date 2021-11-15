#include "rigidBody.h"

#include "physics/collisions/aabb.h"

#include "log/log.h"

void addAABB(Body* body, float halfWidth, float halfHeight)
{
    if (body->aabbID != -1)
        LOG_WARN("Body already has an AABB\n");

    glm::vec2 center = {body->position.x, body->position.y};
    glm::vec2 halfExtents = {halfWidth, halfHeight};

    body->aabbID = createAABB2(center, halfExtents, body);
}
