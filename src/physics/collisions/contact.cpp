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

    int aOnTop = (a->position.y - b->position.y) > 0 ? -1 : 1;
    glm::vec2 separation;
    separation.x = fabs(b->position.x - a->position.x);
    separation.y = aOnTop ? fabs(b->position.y - a->position.y) : fabs(a->position.y - b->position.y);
    // penetration = (separation - minSeparation)
    glm::vec2 penetration = separation - minSeparation;

    glm::vec2 normal = {
        (penetration.x > penetration.y), aOnTop*(penetration.x < penetration.y)
    };

    c->normal = normal;
    c->penetration = penetration;

    if (a->onCollision != NULL)
        a->onCollision(a, b);
    if (b->onCollision != NULL)
        b->onCollision(b, a);
}
