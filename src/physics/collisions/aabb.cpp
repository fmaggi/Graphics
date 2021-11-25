#include "aabb.h"

#include "contact.h"
#include "log/log.h"

AABBManager::AABBs AABBManager::aabbs;

AABB* AABBManager::Create(glm::vec2 center, glm::vec2 halfExtents, Body* body)
{
    AABB* aabb = aabbs.aabbs + aabbs.current;
    aabb->body = (Body*) body;

    aabb->min.x = center.x - halfExtents.x;
    aabb->min.y = center.y - halfExtents.y;
    aabb->max.x = center.x + halfExtents.x;
    aabb->max.y = center.y + halfExtents.y;
    aabb->radius = halfExtents;

    aabbs.sorted[aabbs.current++] = aabb;
    return aabb;
}

void AABBManager::Sort()
{
    for (uint32_t i = 1; i < aabbs.current; i++)
    {
        AABB* ith = aabbs.sorted[i];
        int32_t j = i-1;
        AABB* jth = aabbs.sorted[j];
        float min = ith->min.x;

        while (j >= 0 && jth->min.x > min)
        {
            aabbs.sorted[j+1] = jth;
            jth = aabbs.sorted[--j];
        }
        aabbs.sorted[j+1] = ith;
    }
}

void AABBManager::SweepAndPrune(ContactStack& results)
{
    Sort();
    int active[32] = {-1};
    int index = 0;
    Contact* c = results.contacts;
    c->prev = NULL;
    for (int i = 0; i < aabbs.current; i++)
    {
        active[index] = i;
        for (int j = 0; j < index; j++) // check from right to left if it intersects anything in the interval.
        {
            if (active[j] == -1)
                continue;
            AABB* b = aabbs.sorted[active[index]];
            AABB* a = aabbs.sorted[active[j]];
            if (b->min.x < a->max.x)
            {
                c->left = a->body;
                c->right = b->body;
                c->minSeparation.x = fabs(a->radius.x + b->radius.x);
                c->minSeparation.y = fabs(a->radius.y + b->radius.y);

                c->next = results.contacts + (++results.count);
                c->next->prev = c;
                c = c->next;

                if (results.count == results.size)
                {
                    LOG_WARN("Reached contact limit");
                    return;
                }
            }
            else
            {
                active[j] = -1;
            }
        }
        index++;
    }
}

bool AABBManager::TestOverlap(AABB* a, AABB* b)
{
    return a->min.x < b->max.x && a->max.x > b->min.x
        && a->min.y < b->max.y && a->max.y > b->min.y;
}


void AABB::Update(glm::vec3 translation)
{
    float hWidth = radius.x;
    float hHeihgt = radius.y;

    min = {translation.x - hWidth, translation.y - hHeihgt};
    max = {translation.x + hWidth, translation.y + hHeihgt};
}

Body* AABBManager::QueryOverlap(AABB* aabb)
{
    return nullptr;
}
