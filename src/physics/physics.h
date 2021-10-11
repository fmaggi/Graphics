#ifndef PHYSICS_H
#define PHYSICS_H

#define ACTIVE 0b1
#define INACTIVE 0
#define DYNAMIC 0b10
#define STATIC 0

struct AABB
{
    vec2s min, max;
    vec3s center;
    EntityID e;
};

struct PhysicsObject
{
    vec3s position, scale, speed, force;
    struct AABB aabb;
    char flags;
};

void initPhysics(int width, int height);
void setPhysicsWidthHeight(int width, int height);

void updatePhysics(double ts);

#endif