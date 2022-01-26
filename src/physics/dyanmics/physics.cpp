#include "physics.h"

#include "rigidBody.h"
#include "physics/collisions/aabb.h"
#include "physics/collisions/contact.h"

#include "physics/collisions/broad_phase.h"

#include "log/log.h"

#include "solver.h"

struct Allocator
{
    Body* memory;
    size_t bodyCount = 0;
    Body* freeBodies = nullptr;
};

static Contact* NarrowPhase(Contact* contacts)
{
    int calls = 0;
    Contact* root = contacts;
    Contact* c = root;
    while(c)
    {
        Body* a = c->left;
        Body* b = c->right;
        if (b->type == BodyType::Static && a->type == BodyType::Static)
        {
            Contact* d = c;
            c = c->next;
            DestroyContact(d, &root);
            continue;
        }

        calls++;
        // bool collided = TestOverlap(a->aabb, b->aabb);
        // if (!collided)
        // {
        //     Contact* d = c;
        //     c = c->next;
        //     DestroyContact(d, &root);
        //     continue;
        // }

        collide(c);
        c = c->next;
    }
    LOG_INFO_DEBUG("collide calls: %i", calls);
    return root;
}

PhysicsWorld::PhysicsWorld(size_t maxBodies)
{
    allocator = new Allocator;
    allocator->memory = new Body[maxBodies];
    for (size_t i = 0; i < maxBodies-1; ++i)
    {
        Body* b = allocator->memory + i;
        b->next = allocator->memory + i + 1;
    }
    allocator->freeBodies = allocator->memory;

    broad_phase_data = new BroadPhaseData(maxBodies);
}

PhysicsWorld::~PhysicsWorld()
{
    delete[] allocator->memory;
    delete allocator;
    delete broad_phase_data;
}

void PhysicsWorld::Step(float ts)
{
    Contact* contacts = BroadPhase(broad_phase_data);
    // This part of the code was inspired by erin catto's Box2d physiscs engine. (https://www.box2d.org/)
    contacts = NarrowPhase(contacts);

    Solve(bodies, contacts, gravity, ts);
}

Body* PhysicsWorld::CreateBody(glm::vec2 translation, float mass, BodyType type, CollisionCallback callback, void* userData, uint32_t userFlags)
{
    if (!allocator->freeBodies)
        return nullptr;
    Body* body = allocator->freeBodies;

    body->translation = translation;
    body->mass = mass;
    body->Imass = mass ? (1 / mass) : 0;
    body->type = type;
    body->onCollision = callback;
    body->userData = userData;
    body->userFlags = userFlags;

    body->force = {0, 0};
    body->velocity = {0, 0};
    body->aabb = nullptr;

    allocator->bodyCount++;
    allocator->freeBodies = body->next;

    body->next = bodies;
    bodies = body;
    return body;
}

Body* PhysicsWorld::CreateBody(BodyDef& body)
{
    return CreateBody(body.translation, body.mass, body.type, body.onCollision, body.userData, body.userFlags);
}

void PhysicsWorld::AddAABB(Body* body, float halfWidth, float halfHeight)
{
    if (body->aabb)
        LOG_WARN("Body already has an AABB\n");

    glm::vec2 center = {body->translation.x, body->translation.y};
    glm::vec2 halfExtents = {halfWidth, halfHeight};

    AABB* aabb = broad_phase_data->NewAABB();
    aabb->body = body;
    aabb->min = center - halfExtents;
    aabb->max = center + halfExtents;
    aabb->radius = halfExtents;

    body->aabb = aabb;
}


