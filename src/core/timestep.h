#ifndef TS_H
#define TS_H

#include "stdint.h"

struct TimeStep
{
    float dt = 0;
    float lastFrameTime = 0;

    uint32_t frameCount = 0;

    void Step();
};

#endif
