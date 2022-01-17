#ifndef CONTACT_H
#define CONTACT_H

#include "physics/dyanmics/rigidBody.h"

#include "glm/glm.hpp"

struct Contact
{
    Body *left = nullptr, *right = nullptr;
    glm::vec2 normal, minSeparation;
    Contact *prev, *next;
    float normalMass;
};

struct ContactAllocator
{
    ContactAllocator(uint32_t size = 1000)
        : alloc(new Contact[size]), size(size) {}
    ~ContactAllocator();

    Contact* NewContact();

    Contact* unused = nullptr;
    Contact* alloc = nullptr;
    uint32_t count = 0, size = 0;
};

void reset_contact_allocator(ContactAllocator* stack);

void DestroyContact(Contact* contact, Contact** root);

void collide(Contact* c);

glm::vec2 getPenetration(Body* a, Body* b, glm::vec2 minSeparation);

#endif
