#ifndef BROAD_PHASE_H
#define BROAD_PHASE_H

#include "aabb.h"
#include "contact.h"

struct BroadPhaseData
{
    BroadPhaseData(uint32_t size);
    ~BroadPhaseData();
    AABB* NewAABB();
    // void Free(AABB* aabb);

    AABB** aabbs = nullptr;

    AABB* alloc_aabbs = nullptr;
    AABB* free_aabbs = nullptr;

    uint32_t count = 0;

    ContactAllocator contacts;
};

Contact* BroadPhase(BroadPhaseData* data);

#endif
