#ifndef PHYSICS_H
#define PHYSICS_H

#include "glm/glm.hpp"

#include "rigidBody.h"

typedef void (*CollisionCallback)(Body* self, Body* other);

struct BodyDef
{
    BodyType type = BodyType::Static;
    glm::vec2 translation{0,0};
    float mass;

    uint32_t userFlags = 0;
    void* userData = nullptr;
    CollisionCallback onCollision = nullptr;
};

struct PhysicsWorld
{
    glm::vec2 gravity = {0,0};

    PhysicsWorld(size_t maxBodies = 2000);
    ~PhysicsWorld();

    // steps are better with a fixed ts
    void Step(float ts);

    Body* CreateBody(glm::vec2 translation, float mass, BodyType type, CollisionCallback callback=0, void* userData=0, uint32_t userFlags=0);
    Body* CreateBody(BodyDef& body);

    void AddAABB(Body* body, float halfWidth, float halfHeight);

private:
    struct Allocator* allocator = nullptr;
    struct BroadPhaseData* broad_phase_data = nullptr;
    Body* bodies = nullptr;
};

#endif
