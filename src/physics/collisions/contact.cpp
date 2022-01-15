#include "contact.h"
#include "string.h"

#include "log/log.h"

void reset_contact_stack(ContactStack* stack, uint32_t size)
{
    stack->size = size;
    stack->count = 0;

    delete[] stack->alloc;
    stack->alloc = new Contact[size];

    stack->alloc->prev = nullptr;
    stack->alloc->next = stack->alloc + 1;
    for (int i = 1; i < size-1; i++)
    {
        Contact* c = stack->alloc + i;
        c->next = stack->alloc + i + 1;
        c->prev = stack->alloc + i - 1;
    }

    stack->unused = stack->alloc;
    stack->contacts = nullptr;
}

Contact* ContactStack::NewContact()
{
    if (!unused)
        return nullptr;

    count++;

    // get contact from unused link list
    Contact* c = unused;
    unused = c->next;
    unused->prev = nullptr;

    // add contact to used linked list
    c->next = contacts;
    if (contacts)
        contacts->prev = c;
    c->prev = nullptr;
    contacts = c;
    return c;
}

void ContactStack::DestroyContact(Contact* c)
{
    if (c->prev)
    {
        c->prev->next = c->next;
    }
    if (c->next)
    {
        c->next->prev = c->prev;
    }
    if (c == contacts)
    {
        contacts = c->next;
    }


    count--;
    c->next = unused;
    c->prev = nullptr;
    if (unused)
        unused->prev = c;
    unused = c;
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
