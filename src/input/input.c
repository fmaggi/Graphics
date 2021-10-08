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
    LOG_TRACE("Initaliazing input\n");
    inputs.up    = KEY_W;
    inputs.down  = KEY_S;
    inputs.left  = KEY_A;
    inputs.right = KEY_D;
}

void handleInput(EntityID movable, double timestep)
{
    TransformComponent* t = getComponent(movable, transform);
    if (windowIsKeyPressed(inputs.up))
    {
        vec2s amount;
        amount.x = 0;
        amount.y = 1;
        amount = glms_vec2_scale(amount, timestep);
        t->position = glms_vec2_add(amount, t->position);
    }
    else if (windowIsKeyPressed(inputs.down))
    {
        vec2s amount;
        amount.x = 0;
        amount.y = -1;
        amount = glms_vec2_scale(amount, timestep);
        t->position = glms_vec2_add(amount, t->position);
    }

    if (windowIsKeyPressed(inputs.left))
    {
        vec2s amount;
        amount.x = -1;
        amount.y = 0;
        amount = glms_vec2_scale(amount, timestep);
        t->position = glms_vec2_add(amount, t->position);
    }
    else if (windowIsKeyPressed(inputs.right))
    {
        vec2s amount;
        amount.x = 1;
        amount.y = 0;
        amount = glms_vec2_scale(amount, timestep);
        t->position = glms_vec2_add(amount, t->position);
    }
}

int isKeyPressed(Keycode key)
{
    return windowIsKeyPressed(key);
}
int isMouseButtonPressed(ButtonCode button)
{
    return windowIsMouseButtonPressed(button);
}