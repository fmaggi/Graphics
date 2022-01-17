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
    reset_contact_allocator(&(data->contacts));
    Contact* c = nullptr;
    Contact* root = nullptr;
    int active[32] = {-1};
    int index = 0;
    for (int i = 0; i < data->count; i++)
    {
        active[index] = i;
        for (int j = 0; j < index; j++)
        {
            if (active[j] == -1)
                continue;
            AABB* b = data->aabbs[active[index]];
            AABB* a = data->aabbs[active[j]];
            if (b->min.x < a->max.x)
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

                c->left = a->body;
                c->right = b->body;
                c->minSeparation.x = fabs(a->radius.x + b->radius.x);
                c->minSeparation.y = fabs(a->radius.y + b->radius.y);

            }
            else
            {
                active[j] = -1;
            }
        }
        index++;
    }
    return root;
}
