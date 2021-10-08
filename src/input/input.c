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

void handleInput(EntityID movable, double timestep)
{
    TransformComponent* t = getComponent(movable, transform);
    if (isKeyPressed(inputs.up))
    {
        vec2s amount;
        amount.x = 0;
        amount.y = 1;
        amount = glms_vec2_scale(amount, timestep);
        t->position = glms_vec2_add(amount, t->position);
    }
    else if (isKeyPressed(inputs.down))
    {
        vec2s amount;
        amount.x = 0;
        amount.y = -1;
        amount = glms_vec2_scale(amount, timestep);
        t->position = glms_vec2_add(amount, t->position);
    }

    if (isKeyPressed(inputs.left))
    {
        vec2s amount;
        amount.x = -1;
        amount.y = 0;
        amount = glms_vec2_scale(amount, timestep);
        t->position = glms_vec2_add(amount, t->position);
    }
    else if (isKeyPressed(inputs.right))
    {
        vec2s amount;
        amount.x = 1;
        amount.y = 0;
        amount = glms_vec2_scale(amount, timestep);
        t->position = glms_vec2_add(amount, t->position);
    }
}