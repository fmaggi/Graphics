#include "physics.h"

#include "physics/collisions/aabb.h"
#include "physics/collisions/contact.h"

#include "log/log.h"

struct Simulation
{
    Body bodies[32];
    uint32_t currentBody;
    int32_t gravity;
};

static struct Simulation simulation;

void initPhysics(float gravity)
{
    simulation.currentBody = 0;
    simulation.gravity = gravity;
}

void stepPhysics(double ts)
{
    static struct ContactStack stack;
    struct Contact contacts[32] = {0};
    stack.contacts = contacts;
    stack.count = 0;
    stack.size = 32;

    for (int i = 0; i < simulation.currentBody; i++)
    {
        Body* b = simulation.bodies + i;
        updateAABB(b->aabbID, b->position);
    }

    sweepAndPrune(&stack);

    int calls = 0;
    for (struct Contact* c = stack.contacts; c->next != NULL; c = c->next)
    {
        Body* a = c->left;
        Body* b = c->right;
        if (b->type == Static && a->type == Static)
        {
            struct Contact* d = c;
            destroyContact(&stack, d);
            continue;
        }

        calls++;
        bool collided = testOverlap(a->aabbID, b->aabbID);
        if (!collided)
        {
            struct Contact* d = c;
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
        if (b->type == Static)
            continue;

        b->impulse.y += simulation.gravity;

        glm::vec2 speed = (float) ts * b->impulse;
        b->speed += speed;

        b->impulse = {0, 0};
    }

    // solve velocity constraints
    for (struct Contact* c = stack.contacts; c->next != NULL; c = c->next)
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

        if (a->type == Dynamic)
        {
            a->speed += p;
        }
        if (b->type == Dynamic)
        {
            b->speed -= p;
        }
    }

    // integrate position
    for (int i = 0; i < simulation.currentBody; i ++)
    {
        Body* b = simulation.bodies + i;
        if (b->type == Static)
            continue;

        glm::vec2 dx = (float)ts * b->speed;
        b->position += (glm::vec3){dx.x, dx.y, 0};
    }

    // solve position constraints
    for (struct Contact* c = stack.contacts; c->next != NULL; c = c->next)
    {
        Body *a = c->left;
        Body *b = c->right;

        glm::vec2 normal = c->normal;
        glm::vec2 penetration = c->penetration;

        glm::vec2 offset = penetration * normal;

        if (a->type == Dynamic)
        {
            a->position += (glm::vec3){offset.x, offset.y, 0};
        }
        if (b->type == Dynamic)
        {
            b->position -= (glm::vec3){offset.x, offset.y, 0};
        }
    }
}

Body* createBody(glm::vec3 position, enum BodyType type, CollisionCallback callback, void* userData, uint32_t userFlags)
{
    Body* body= simulation.bodies + simulation.currentBody++;

    body->position = position;
    body->type = type;
    body->onCollision = callback;
    body->userData = userData;
    body->userFlags = userFlags;

    body->impulse = {0, 0};
    body->speed = {0, 0};
    body->aabbID = -1;

    return body;
}

