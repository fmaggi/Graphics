#include "physics.h"

#include "rigidBody.h"
#include "physics/collisions/aabb.h"
#include "physics/collisions/contact.h"

#include "log/log.h"

#define VEL_ITERATIONS 5
#define POS_ITERATIONS 2

struct Simulation
{
    Body* bodies;
    uint32_t currentBody;
    glm::vec2 gravity;
};
static Simulation simulation;

void Physics::Init(float gravity)
{
    static Body bodies[32];
    simulation.currentBody = 0;
    simulation.gravity = { 0, gravity };
    simulation.bodies = bodies;
}

void Physics::Step(float ts)
{
    static ContactStack stack;
    Contact contacts[10000] = {0};
    stack.contacts = contacts;
    stack.count = 0;
    stack.size = 10000;

    for (int i = 0; i < simulation.currentBody; i++)
    {
        Body* b = simulation.bodies + i;
        b->aabb->Update(b->translation);
    }

    AABBManager::SweepAndPrune(stack);

    int calls = 0;
    for (Contact* c = stack.contacts; c->next != NULL; c = c->next)
    {
        Body* a = c->left;
        Body* b = c->right;
        if (b->type == BodyType::Static && a->type == BodyType::Static)
        {
            Contact* d = c;
            destroyContact(&stack, d);
            continue;
        }

        calls++;
        bool collided = AABBManager::TestOverlap(a->aabb, b->aabb);
        if (!collided)
        {
            Contact* d = c;
            destroyContact(&stack, d);
            continue;
        }
        collide(c);
    }
    LOG_INFO_DEBUG("collide calls: %i\n", calls);

    // This part of the code was inspired by erin catto's Box2d physiscs engine. (https://www.box2d.org/)

    //integrate velocity
    for (int i = 0; i < simulation.currentBody; i++)
    {
        Body* b = simulation.bodies + i;
        if (!b->mass || b->type == BodyType::Static)
            continue;

        glm::vec2 v = b->velocity;
        v += ts * (simulation.gravity + b->force * b->Imass);


        v *= 1.0f / (1.0f + ts);

        b->velocity = v;
        b->force = {0, 0};
    }

    // solve velocity constraints
    for (Contact* c = stack.contacts; c->next != NULL; c = c->next)
    {
        Body *a = c->left;
        Body *b = c->right;

        glm::vec2 normal = c->normal;
        glm::vec2 tangent = {
            -normal.y, normal.x
        };

        for (int j = 0; j < VEL_ITERATIONS; j++)
        {
            glm::vec2 dv = b->velocity - a->velocity;
            if (glm::dot(dv, normal) > 0.0f)
                continue;

            float tangentImpulse = -glm::dot(dv, tangent) * 0; // friction
            glm::vec2 pt = tangent * tangentImpulse;

            float normalImpulse = -glm::dot(dv, normal) * c->normalMass * 1.3; // restitution
            normalImpulse = std::max(normalImpulse, 0.0f);
            glm::vec2 pn = normal * normalImpulse;

            glm::vec2 p = pt + pn;

            a->velocity -= p * a->Imass;
            b->velocity += p * b->Imass;
        }
    }

    // integrate translation
    for (int i = 0; i < simulation.currentBody; i ++)
    {
        Body* b = simulation.bodies + i;
        if (b->type == BodyType::Static)
            continue;

        glm::vec2 dx = ts * b->velocity;
        b->translation += dx;
    }

    // solve translation constraints
    for (Contact* c = stack.contacts; c->next != NULL; c = c->next)
    {
        float minPenetration = 0.0f;
        Body *a = c->left;
        Body *b = c->right;

        glm::vec2 normal = c->normal;

        for (int i = 0; i < POS_ITERATIONS; i++)
        {
            glm::vec2 penetration = getPenetration(a, b, c->minSeparation);
            float penetrationNormal = glm::dot(penetration, normal);

            minPenetration = std::min(minPenetration, penetrationNormal);

            glm::vec2 res = minPenetration * normal;

            if (a->type == BodyType::Dynamic)
            {
                a->translation += res;
            }
            if (b->type == BodyType::Dynamic)
            {
                b->translation -= res;
            }

        }
    }
}

Body* Physics::CreateBody(glm::vec2 translation, float mass, BodyType type, CollisionCallback callback, void* userData, uint32_t userFlags)
{
    Body* body= simulation.bodies + simulation.currentBody++;

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

    return body;
}

Body* Physics::CreateBody(BodyDef& body)
{
    return CreateBody(body.translation, body.mass, body.type, body.onCollision, body.userData, body.userFlags);
}

Body* Physics::QueryContact(Body* body)
{
    body->aabb->Update(body->translation);
    return (Body*) AABBManager::QueryOverlap(body->aabb);
}

void Physics::AddAABB(Body* body, float halfWidth, float halfHeight)
{
    if (body->aabb)
        LOG_WARN("Body already has an AABB\n");

    glm::vec2 center = {body->translation.x, body->translation.y};
    glm::vec2 halfExtents = {halfWidth, halfHeight};

    body->aabb = AABBManager::Create(center, halfExtents, body);
}


