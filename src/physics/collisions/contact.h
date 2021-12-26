#ifndef CONTACT_H
#define CONTACT_H

#include "physics/dyanmics/rigidBody.h"

#include "glm/glm.hpp"

struct Contact
{
    Body *left, *right;
    glm::vec2 normal, minSeparation;
    struct Contact *prev, *next;
    float normalMass;
};

struct ContactStack
{
    struct Contact* contacts;
    uint32_t count, size;
};

void destroyContact(struct ContactStack* stack, struct Contact* c);
void collide(struct Contact* c);

glm::vec2 getPenetration(Body* a, Body* b, glm::vec2 minSeparation);

#endif
