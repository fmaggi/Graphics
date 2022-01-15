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

struct ContactStack
{
    Contact* NewContact();
    void DestroyContact(Contact* contact);

    Contact* contacts = nullptr;
    Contact* unused = nullptr;
    Contact* alloc = nullptr;
    uint32_t count = 0, size = 0;
};

void reset_contact_stack(ContactStack* stack, uint32_t size);

void collide(Contact* c);

glm::vec2 getPenetration(Body* a, Body* b, glm::vec2 minSeparation);

#endif
