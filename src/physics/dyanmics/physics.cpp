#include "physics.h"

#include "rigidBody.h"
#include "physics/collisions/aabb.h"
#include "physics/collisions/contact.h"

#include "physics/collisions/broad_phase.h"

#include "log/log.h"

#define VEL_ITERATIONS 5
#define POS_ITERATIONS 2

static float max(float a, float b)
{
    return a > b ? a : b;
}

static void integrate_forces(Body* bodies, glm::vec2 gravity, float ts)
{
    Body* b = bodies;
    while (b != nullptr)
    {
        if (!b->mass || b->type == BodyType::Static)
        {
            b = b->next;
            continue;
        }

        glm::vec2 v = b->velocity;
        v += ts * (gravity + b->force * b->Imass);

        b->velocity = v;
        b->force = {0, 0};

        b = b->next;
    }
}

static void integrate_velocities(Body* bodies, float ts)
{
    Body* b = bodies;
    while (b != nullptr)
    {
        if (b->type == BodyType::Static)
        {
            b = b->next;
            continue;
        }

        glm::vec2 dx = ts * b->velocity;
        b->translation += dx;
        b = b->next;
    }
}

static void solve_velocity_constraints(Contact* contact)
{
    Contact* c = contact;
    while (c)
    {
        Body *a = c->left;
        Body *b = c->right;

        glm::vec2 normal = c->normal;
        glm::vec2 tangent = {
            -normal.y, normal.x
        };
        float normalMass = c->normalMass;

        glm::vec2 dv = b->velocity - a->velocity;

        float tangentImpulse = -glm::dot(dv, tangent) * 0.3; // friction
        glm::vec2 pt = tangent * tangentImpulse;

        float dvn = glm::dot(dv, normal);
        float normalImpulse = dvn * -normalMass * 2; // restitution

        float newImpulse = max(normalImpulse + c->normalImpulse, 0.0f);
        normalImpulse = newImpulse - c->normalImpulse;
        c->normalImpulse = newImpulse;

        glm::vec2 pn = normal * normalImpulse;

        glm::vec2 p = pt + pn;

        a->velocity -= p * a->Imass;
        b->velocity += p * b->Imass;
        c = c->next;
    }
}

static void solve_position_constraints(Contact* contacts)
{
    Contact* c = contacts;
    while(c)
    {
        float minPenetration = 0.0f;
        Body *a = c->left;
        Body *b = c->right;

        glm::vec2 normal = c->normal;
        float K = 1 / c->normalMass;

        glm::vec2 penetration = getPenetration(a, b, c->minSeparation);

        float penetrationNormal = fabs(glm::dot(penetration, normal));

        minPenetration = std::min(minPenetration, -penetrationNormal);

        // black magic from erin catto at box2d. Dont know why we divide penetration with normal mass
        // this fixes "vibrations" when one box stands on another
        // I guess is to make them separate from each other relative to their masses
        // also, why is it an impulse?
        float impulse = K > 0.0f ? minPenetration / K : 0.0f;

        glm::vec2 res = impulse * normal;

        a->translation += res * a->Imass;
        b->translation -= res * b->Imass;

        c = c->next;
    }
}

static Contact* filter_contacts(Contact* contacts)
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
    : memory(new Body[maxBodies])
{
    for (size_t i = 0; i < maxBodies-1; ++i)
    {
        Body* b = memory + i;
        b->next = memory + i + 1;
    }
    freeBodies = memory;

    broad_phase_data = new BroadPhaseData(maxBodies);
}

void PhysicsWorld::Step(float ts)
{
    for (Body* b = bodies; b != nullptr; b = b->next)
    {
        b->aabb->Update(b->translation);
    }

    Contact* contacts = BroadPhase(broad_phase_data);
    // This part of the code was inspired by erin catto's Box2d physiscs engine. (https://www.box2d.org/)
    contacts = filter_contacts(contacts);

    integrate_forces(bodies, gravity, ts);

    for (int _ = 0 ; _ < VEL_ITERATIONS; ++_)
        solve_velocity_constraints(contacts);

    integrate_velocities(bodies, ts);

    for (int _ = 0 ; _ < POS_ITERATIONS; ++_)
        solve_position_constraints(contacts);
}

Body* PhysicsWorld::CreateBody(glm::vec2 translation, float mass, BodyType type, CollisionCallback callback, void* userData, uint32_t userFlags)
{
    if (!freeBodies)
        return nullptr;
    Body* body = freeBodies;

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

    bodyCount++;
    freeBodies = body->next;

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


