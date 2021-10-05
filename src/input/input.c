#include "input.h"

#include "graphics/window.h"

#include "cglm/struct.h"

#include "log/log.h"

struct _inputs
{
    unsigned int up;
    unsigned int down;
    unsigned int left;
    unsigned int right;
    GLFWwindow* window;
};

static struct _inputs inputs;

void initInput()
{
    inputs.up    = GLFW_KEY_W;
    inputs.down  = GLFW_KEY_S;
    inputs.left  = GLFW_KEY_A;
    inputs.right = GLFW_KEY_D;
    inputs.window = getNativeWindow();
    if (inputs.window == NULL)
        LOG_WARN("Window hasn't been created\n");
}

void handleInput(Entity* movable, double timestep)
{
    if (glfwGetKey(inputs.window, inputs.up) == GLFW_PRESS)
    {
        vec3s amount;
        amount.x = 0;
        amount.y = 100;
        amount.z = 0;
        amount = glms_vec3_scale(amount, timestep);
        movable->pos = glms_vec3_add(amount, movable->pos);
    }
    else if (glfwGetKey(inputs.window, inputs.down) == GLFW_PRESS)
    {
        vec3s amount;
        amount.x = 0;
        amount.y = -100;
        amount.z = 0;
        amount = glms_vec3_scale(amount, timestep);
        movable->pos = glms_vec3_add(amount, movable->pos);
    }

    if (glfwGetKey(inputs.window, inputs.left) == GLFW_PRESS)
    {
        vec3s amount;
        amount.x = -100;
        amount.y = 0;
        amount.z = 0;
        amount = glms_vec3_scale(amount, timestep);
        movable->pos = glms_vec3_add(amount, movable->pos);
    }
    else if (glfwGetKey(inputs.window, inputs.right) == GLFW_PRESS)
    {
        vec3s amount;
        amount.x = 100;
        amount.y = 0;
        amount.z = 0;
        amount = glms_vec3_scale(amount, timestep);
        movable->pos = glms_vec3_add(amount, movable->pos);
    }
}