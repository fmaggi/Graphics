#ifndef PHYSICS_H
#define PHYSICS_H

#include "glm/glm.hpp"

struct Body;
enum class BodyType
{
    Static = 0, Dynamic
};

typedef void (*CollisionCallback)(Body* self, Body* other);

struct BodyDef
{
    BodyType type = BodyType::Static;
    glm::vec3 translation{0,0,0};

    uint32_t userFlags = 0;
    void* userData = nullptr;
    CollisionCallback onCollision = nullptr;
};

class Physics
{
public:
    static void Init(float gravity);
    static void Step(float ts);

    static Body* CreateBody(glm::vec3 translation, BodyType type, CollisionCallback callback=0, void* userData=0, uint32_t userFlags=0);
    static Body* CreateBody(BodyDef& body);

    static Body* QueryContact(Body* body);
    static void AddAABB(Body* body, float halfWidth, float halfHeight);
};

#endif
