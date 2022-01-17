#include "contact.h"
#include "string.h"

#include "log/log.h"

void reset_contact_allocator(ContactAllocator* stack)
{
    stack->count = 0;

    stack->alloc->prev = nullptr;
    stack->alloc->next = stack->alloc + 1;
    for (int i = 1; i < stack->size-1; i++)
    {
        Contact* c = stack->alloc + i;
        c->next = stack->alloc + i + 1;
        c->prev = stack->alloc + i - 1;
    }
    stack->unused = stack->alloc;
}

// ContactAllocator::ContactAllocator(ContactAllocator&& other)
// {
//     LOG_INFO("MOVE");
//     this->alloc = other.alloc;
//     other.alloc = nullptr;

//     this->contacts = other.contacts;
//     other.contacts = nullptr;

//     this->unused = other.unused;
//     other.unused = nullptr;

//     this->count = other.count;
//     this->size = other.size;
// }

ContactAllocator::~ContactAllocator()
{
    delete[] alloc;
}

Contact* ContactAllocator::NewContact()
{
    if (!unused)
        return nullptr;

    count++;

    // get contact from unused link list
    Contact* c = unused;
    unused = c->next;
    unused->prev = nullptr;

    // add contact to used linked list
    return c;
}

void DestroyContact(Contact* c, Contact** root)
{
    if (c->prev)
    {
        c->prev->next = c->next;
    }
    if (c->next)
    {
        c->next->prev = c->prev;
    }
    if (c == *root)
    {
        *root = c->next;
    }
}

void collide(struct Contact* c)
{
    Body* a = c->left;
    Body* b = c->right;

    glm::vec2 minSeparation = c->minSeparation;

    int aOnTop = (a->translation.y - b->translation.y) > 0 ? -1 : 1;

    glm::vec2 penetration = getPenetration(a, b, minSeparation);

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
    glm::vec2 separation;
    separation.x = fabs(b->translation.x - a->translation.x);
    separation.y = fabs(b->translation.y - a->translation.y);
    // penetration = (separation - minSeparation)
    glm::vec2 penetration = separation - minSeparation;
    return penetration;
}
