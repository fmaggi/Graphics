#ifndef AABB_H
#define AABB_H

#include "glm/glm.hpp"

class Body;
struct ContactStack;

struct AABB
{
    void Update(glm::vec3 translation);

    glm::vec2 min, max, radius;
    Body* body;
};

class AABBManager
{
public:
    static AABB* Create(glm::vec2 center, glm::vec2 halfExtents, Body* bodyID);
    static void SweepAndPrune(ContactStack& results);
    static bool TestOverlap(AABB* a, AABB* b);
    static Body* QueryOverlap(AABB* aabb);
private:
    static void Sort();
    struct AABBs
    {
        AABB aabbs[32];
        AABB* sorted[32];
        uint32_t current = 0;
    };
    static AABBs aabbs;
};

#endif
