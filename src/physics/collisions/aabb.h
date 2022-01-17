#ifndef AABB_H
#define AABB_H

#include "glm/glm.hpp"

struct Body;

struct AABB
{
    void Update(glm::vec2 translation);

    glm::vec2 min, max, radius;
    Body* body;
    AABB* next = nullptr;
    AABB* prev = nullptr;
};

inline bool TestOverlap(AABB* a, AABB* b)
{
    return a->min.x < b->max.x && a->max.x > b->min.x
        && a->min.y < b->max.y && a->max.y > b->min.y;
}

#endif
