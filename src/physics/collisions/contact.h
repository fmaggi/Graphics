#ifndef CONTACT_H
#define CONTACT_H

#include "physics/dyanmics/rigidBody.h"
#include "cglm/struct.h"

struct Contact
{
    Body *left, *right;
    vec2s normal, minSeparation, penetration;
    struct Contact *prev, *next;
};

struct ContactStack
{
    struct Contact* contacts;
    uint32_t count, size;
};


void destroyContact(struct ContactStack* stack, struct Contact* c);
void collide(struct Contact* c);

#endif
