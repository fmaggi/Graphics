#include "timestep.h"

#include "graphics/gfx.h"

void TimeStep::Step()
{
    float now = (float) glfwGetTime();
    dt = now - lastFrameTime;
    lastFrameTime = now;

    ++frameCount;
}
