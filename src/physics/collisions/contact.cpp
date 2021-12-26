#include "contact.h"
#include "string.h"

void destroyContact(struct ContactStack* stack, struct Contact* c)
{
    if (c->prev)
    {
        c->prev->next = c->next;
    }
    if (c->next)
    {
        c->next->prev = c->prev;
    }
    if (c == stack->contacts)
    {
        stack->contacts = c->next;
        stack->size -= 1;
    }
}

void collide(struct Contact* c)
{
    Body* a = c->left;
    Body* b = c->right;

    glm::vec2 minSeparation = c->minSeparation;

    int aOnTop = (a->translation.y - b->translation.y) > 0 ? -1 : 1;
    glm::vec2 separation;
    separation.x = fabs(b->translation.x - a->translation.x);
    separation.y = aOnTop ? fabs(b->translation.y - a->translation.y) : fabs(a->translation.y - b->translation.y);
    // penetration = (separation - minSeparation)
    glm::vec2 penetration = separation - minSeparation;

    glm::vec2 normal = {
        (penetration.x >= penetration.y), aOnTop*(penetration.x < penetration.y)
    };

    c->normalMass = 1/ (a->Imass + b->Imass);

    c->normal = normal;

    if (a->onCollision)
        a->onCollision(a, b);
    if (b->onCollision)
        b->onCollision(b, a);
}

glm::vec2 getPenetration(Body* a, Body* b, glm::vec2 minSeparation)
{
    int aOnTop = (a->translation.y - b->translation.y) > 0 ? -1 : 1;
    glm::vec2 separation;
    separation.x = fabs(b->translation.x - a->translation.x);
    separation.y = aOnTop ? fabs(b->translation.y - a->translation.y) : fabs(a->translation.y - b->translation.y);
    // penetration = (separation - minSeparation)
    glm::vec2 penetration = separation - minSeparation;
    return penetration;
}
