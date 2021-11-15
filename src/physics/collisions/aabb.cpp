#include "aabb.h"

#include "contact.h"
#include "log/log.h"

typedef struct
{
    glm::vec2 min, max, radius;
    Body* body;
} AABB;

AABB aabbs[32];
static int current = 0;

int sorted[32] = {0};

void sort()
{
    for (int i = 1; i < current; i++)
    {
        int index = sorted[i];
        int j = i-1;
        int jndex = sorted[j];
        float min = aabbs[index].min.x;

        while (j >= 0 && aabbs[jndex].min.x > min)
        {
            sorted[j+1] = jndex;
            --j;
            jndex = sorted[j];
        }
        sorted[j+1] = index;
    }
}

void sweepAndPrune(struct ContactStack* results)
{
    sort();
    int active[32] = {-1};
    int index = 0;
    struct Contact* c = results->contacts;
    c->prev = NULL;
    for (int i = 0; i < current; i++)
    {
        active[index] = sorted[i];
        for (int j = 0; j < index; j++) // check from right to left if it intersects anything in the interval.
        {
            if (active[j] == -1)
                continue;
            AABB b = aabbs[active[index]];
            AABB a = aabbs[active[j]];
            if (b.min.x < a.max.x)
            {
                c->left = a.body;
                c->right = b.body;
                c->minSeparation.x = fabs(a.radius.x + b.radius.x);
                c->minSeparation.y = fabs(a.radius.y + b.radius.y);

                c->next = results->contacts + (++results->count);
                c->next->prev = c;
                c = c->next;

                if (results->count == results->size)
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

bool testOverlap(int aID, int bID)
{
    AABB* a = aabbs + aID;
    AABB* b = aabbs + bID;
    return a->min.x < b->max.x && a->max.x > b->min.x
        && a->min.y < b->max.y && a->max.y > b->min.y;
}

int32_t createAABB2(glm::vec2 center, glm::vec2 halfExtents, void* body)
{
    AABB* aabb = aabbs + current;
    aabb->body = (Body*) body;

    aabb->min.x = center.x - halfExtents.x;
    aabb->min.y = center.y - halfExtents.y;
    aabb->max.x = center.x + halfExtents.x;
    aabb->max.y = center.y + halfExtents.y;
    aabb->radius = halfExtents;

    sorted[current] = current;
    return current++;
}

void updateAABB(int id, glm::vec3 position)
{
    AABB* a = aabbs + id;

    float hWidth = a->radius.x;
    float hHeihgt = a->radius.y;

    a->min = {position.x - hWidth, position.y - hHeihgt};
    a->max = {position.x + hWidth, position.y + hHeihgt};
}
