#include "physics.h"

#include "physics/collisions/aabb.h"
#include "physics/collisions/contact.h"

#include "log/log.h"

Physics::Simulation Physics::simulation;

void Physics::Init(float gravity)
{
    simulation.currentBody = 0;
    simulation.gravity = gravity;
}

void Physics::Step(float ts)
{
    static ContactStack stack;
    Contact contacts[32] = {0};
    stack.contacts = contacts;
    stack.count = 0;
    stack.size = 32;

    for (int i = 0; i < simulation.currentBody; i++)
    {
        Body* b = simulation.bodies + i;
        updateAABB(b->aabbID, b->translation);
    }

    sweepAndPrune(&stack);

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
        bool collided = testOverlap(a->aabbID, b->aabbID);
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
        if (b->type == BodyType::Static)
            continue;

        b->impulse.y += simulation.gravity;

        glm::vec2 speed = (float) ts * b->impulse;
        b->speed += speed;

        b->impulse = {0, 0};
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

        glm::vec2 dv = b->speed - a->speed;

        float lamdat = glm::dot(dv, tangent) * 0; // friction
        glm::vec2 pt = tangent * lamdat;

        float lamdan = glm::dot(dv, normal) * 1.5; // restitution
        glm::vec2 pn = normal * lamdan;

        glm::vec2 p = pt + pn;

        if (a->type == BodyType::Dynamic)
        {
            a->speed += p;
        }
        if (b->type == BodyType::Dynamic)
        {
            b->speed -= p;
        }
    }

    // integrate translation
    for (int i = 0; i < simulation.currentBody; i ++)
    {
        Body* b = simulation.bodies + i;
        if (b->type == BodyType::Static)
            continue;

        glm::vec2 dx = ts * b->speed;
        b->translation += glm::vec3(dx, 0);
    }

    // solve translation constraints
    for (Contact* c = stack.contacts; c->next != NULL; c = c->next)
    {
        Body *a = c->left;
        Body *b = c->right;

        glm::vec2 normal = c->normal;
        glm::vec2 penetration = c->penetration;

        glm::vec2 offset = penetration * normal;

        if (a->type == BodyType::Dynamic)
        {
            a->translation += (glm::vec3){offset.x, offset.y, 0};
        }
        if (b->type == BodyType::Dynamic)
        {
            b->translation -= (glm::vec3){offset.x, offset.y, 0};
        }
    }
}

Body* Physics::CreateBody(glm::vec3 translation, BodyType type, CollisionCallback callback, void* userData, uint32_t userFlags)
{
    Body* body= simulation.bodies + simulation.currentBody++;

    body->translation = translation;
    body->type = type;
    body->onCollision = callback;
    body->userData = userData;
    body->userFlags = userFlags;

    body->impulse = {0, 0};
    body->speed = {0, 0};
    body->aabbID = -1;

    return body;
}

Body* Physics::CreateBody(BodyDef& body)
{
    return CreateBody(body.translation, body.type, body.onCollision, body.userData, body.userFlags);
}

Body* Physics::QueryContact(Body* body)
{
    updateAABB(body->aabbID, body->translation);
    return (Body*) QueryOverlap(body->aabbID);
}

