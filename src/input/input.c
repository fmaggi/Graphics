#include "input.h"

#include "graphics/window.h"

#include "cglm/struct.h"

#include "log/log.h"

struct _inputs
{
    int up;
    int down;
    int left;
    int right;
};

static struct _inputs inputs;

void initInput()
{
    inputs.up    = KEY_W;
    inputs.down  = KEY_S;
    inputs.left  = KEY_A;
    inputs.right = KEY_D;
}

void handleInput(Entity* movable, double timestep)
{
    if (isKeyPressed(inputs.up))
    {
        vec3s amount;
        amount.x = 0;
        amount.y = 1 * movable->speed.y;
        amount.z = 0;
        amount = glms_vec3_scale(amount, timestep);
        movable->pos = glms_vec3_add(amount, movable->pos);
    }
    else if (isKeyPressed(inputs.down))
    {
        vec3s amount;
        amount.x = 0;
        amount.y = -1 * movable->speed.y;
        amount.z = 0;
        amount = glms_vec3_scale(amount, timestep);
        movable->pos = glms_vec3_add(amount, movable->pos);
    }

    if (isKeyPressed(inputs.left))
    {
        vec3s amount;
        amount.x = -1 * movable->speed.x;
        amount.y = 0;
        amount.z = 0;
        amount = glms_vec3_scale(amount, timestep);
        movable->pos = glms_vec3_add(amount, movable->pos);
    }
    else if (isKeyPressed(inputs.right))
    {
        vec3s amount;
        amount.x = 1 * movable->speed.x;
        amount.y = 0;
        amount.z = 0;
        amount = glms_vec3_scale(amount, timestep);
        movable->pos = glms_vec3_add(amount, movable->pos);
    }
}