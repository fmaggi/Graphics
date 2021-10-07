#include "timestep.h"

#include "graphics/gfx.h"


double getTimestep()
{
    static double lastFrame = 0;
    
    double now = glfwGetTime();
    double ts = now - lastFrame;
    lastFrame = now;
    return ts;
}