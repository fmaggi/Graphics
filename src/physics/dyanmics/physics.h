#ifndef PHYSICS_H
#define PHYSICS_H

#include "rigidBody.h"

class Physics
{
public:
    static void Init(float gravity);
    static void Step(float ts);

    static Body* CreateBody(glm::vec3 translation, BodyType type, CollisionCallback callback=0, void* userData=0, uint32_t userFlags=0);
    static Body* CreateBody(BodyDef& body);

    static Body* QueryContact(Body* body);
private:
    struct Simulation
    {
        Body bodies[32];
        uint32_t currentBody;
        int32_t gravity;
    };
    static Simulation simulation;
};

#endif
