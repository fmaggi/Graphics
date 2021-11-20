#include "timestep.h"

#include "graphics/gfx.h"

float getTimestep()
{
    static float lastFrame = 0;

    float now = (float) glfwGetTime();
    float ts = now - lastFrame;
    lastFrame = now;
    return ts;
}
