#include "rigidBody.h"

#include "physics/collisions/aabb.h"

#include "log/log.h"

void Body::AddAABB(float halfWidth, float halfHeight)
{
    if (aabbID != -1)
        LOG_WARN("Body already has an AABB\n");

    glm::vec2 center = {translation.x, translation.y};
    glm::vec2 halfExtents = {halfWidth, halfHeight};

    aabbID = createAABB2(center, halfExtents, this);
}
