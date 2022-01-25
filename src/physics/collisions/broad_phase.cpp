#include "broad_phase.h"

#include "log/log.h"

BroadPhaseData::BroadPhaseData(uint32_t size)
{
    alloc_aabbs = new AABB[size];
    aabbs = new AABB*[size];

    alloc_aabbs->prev = nullptr;
    alloc_aabbs->next = alloc_aabbs + 1;

    for (int i = 1; i < size-1; ++i)
    {
        AABB* ab = alloc_aabbs + i;
        ab->prev = alloc_aabbs + i - 1;
        ab->next = alloc_aabbs + i + 1;
    }

    free_aabbs = alloc_aabbs;
}

BroadPhaseData::~BroadPhaseData()
{
    delete[] alloc_aabbs;
    delete[] aabbs;
}

AABB* BroadPhaseData::NewAABB()
{
    if (!free_aabbs)
        return nullptr;

    AABB* aabb = free_aabbs;
    free_aabbs = aabb->next;
    if (free_aabbs)
        free_aabbs->prev = nullptr;

    aabb->next = nullptr;
    aabb->prev = nullptr;
    aabbs[count++] = aabb;
    return aabb;
}

// void BroadPhaseData::Free(AABB* aabb)
// {
//     if (aabb->prev)
//         aabb->prev->next = aabb->next;
//     if (aabb->next)
//         aabb->next->prev = aabb->prev;
//     if (aabb == aabbs)
//         aabbs = aabb->next;

//     aabb->prev = nullptr;
//     aabb->next = free_aabbs;
//     free_aabbs = aabb;
//     --count;
// }

static void Sort(AABB** aabbs, uint32_t count)
{
    for (uint32_t i = 1; i < count; i++)
    {
        AABB* ith = aabbs[i];
        int32_t j = i-1;
        AABB* jth = aabbs[j];
        float min = ith->min.x;

        while (j >= 0 && jth->min.x > min)
        {
            aabbs[j + 1] = jth;
            --j;
            jth = aabbs[j];
        }
        aabbs[j + 1] = ith;
    }
}

Contact* BroadPhase(BroadPhaseData* data)
{
    Sort(data->aabbs, data->count);
    AABB** aabbs = data->aabbs;

    reset_contact_allocator(&(data->contacts));

    Contact* c = nullptr;
    Contact* root = nullptr;

    AABB* active_aabbs = aabbs[0];
    float max_x = active_aabbs->max.x;
    active_aabbs->next = nullptr; // clear the next pointer to avoid infinit loop

    for (int i = 1; i < data->count; ++i)
    {
        AABB* a = aabbs[i];
        if (a->min.x < max_x)
        {
            AABB* b = active_aabbs;
            while(b)
            {
                if (TestOverlap(a, b))
                {
                    Contact* temp = data->contacts.NewContact();
                    if (!temp)
                    {
                        LOG_WARN("Reached contact limit");
                        return root;
                    }
                    temp->next = nullptr;
                    temp->prev = c;
                    if (!c)
                        root = temp;
                    else
                        c->next = temp;
                    c = temp;

                    c->left = b->body;
                    c->right = a->body;
                    c->minSeparation.x = fabs(a->radius.x + b->radius.x);
                    c->minSeparation.y = fabs(a->radius.y + b->radius.y);
                }
                b = b->next;
            }
            max_x = a->max.x > max_x ? a->max.x : max_x;
            a->next = active_aabbs;
            active_aabbs = a;
        }
        else
        {
            a->next = nullptr;
            active_aabbs = a;
            max_x = a->max.x;
        }
    }

    return root;
}
