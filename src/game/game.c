#include "game.h"
#include "timestep.h"
#include "input.h"

#include "cglm/struct.h"

#include "events/event.h"

#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/camera.h"

#include "world.h"
#include "entity/entity.h"

#include "log/log.h"

// On event function prototypes

void onWindowClose();
void onWindowResize(WindowResizeEvent event);
void onKeyPressed(KeyEvent event);
void onMouseScrolled(MouseScrollEvent event);
void onMouseMoved(MouseMovedEvent event);

// -----------------------------

int running;

void onEvent(EventHolder* event)
{
    if (event->type == WindowClose)
        return onWindowClose();
    if (event->type == WindowResize)
        return onWindowResize(*(WindowResizeEvent*) event->instance);

    if (onEventWorld(event))
        return;

    switch (event->type)
    {
        case KeyPressed:    return onKeyPressed(*(KeyEvent*) event->instance);
        case KeyReleased:   return;
        case MouseScrolled: return onMouseScrolled(*(MouseScrollEvent*) event->instance);
        case MouseMoved:    return onMouseMoved(*(MouseMovedEvent*) event->instance);

        default:
            LOG_INFO("Event type not currently handled");
            return;
    }
}

void setUpGame()
{
    LOG_INFO_DEBUG("DEBUG");
    createWindow(1200, 800, "LearnOpenGL", &onEvent);
    orthoCamera((vec3s){{0, 0, 0}}, 1200, 800);

    initRenderer();
    initECS();

    LOG_TRACE("Initializing client World");
    initWorld();

    running = 1;
    LOG_TRACE("All done!");
}

void onUpdate()
{
    double ts = getTimestep(); // I did this to abstract this file of anything GLFW / OpenGL related
    LOG_INFO_DEBUG("FPS: %f", 1/ts);

    onUpdateWorld(ts);
}

void onRender()
{
    startFrame();
    onRenderWorld();
    endFrame();
}

void runGame()
{
    while (running)
    {
        onUpdate();
        onRender();
        updateWindow();
    }
}

void destroyGame()
{
    destroyWorld();
    destroyECS();
    destroyRenderer();
    destroyWindow();
    LOG_TRACE("Good bye");
}

void onWindowClose()
{
    running = 0;
}

void onWindowResize(WindowResizeEvent event)
{
    updateProjectionMatrix(event.width, event.height);
}

void onKeyPressed(KeyEvent event)
{
    if (event.repeat)
        return;
    switch (event.key)
    {
        case KEY_M:
        {
            rendererChangeMode();
            break;
        }
        case KEY_C:
        {
            if (event.mods == MOD_CONTROL)
                running = 0;
            else
                rendererSetShader(basicShader);
            break;
        }
        case KEY_U:
        {
            rendererSetShader(uvShader);
            break;
        }
        default:
        {
            break;
        }
    }
}


void onMouseScrolled(MouseScrollEvent event)
{
    updateZoom(event.yoffset);
}

void onMouseMoved(MouseMovedEvent event)
{
    static float lastX = 0;
    static float lastY = 0;

    float offsetX = event.x - lastX;
    float offsetY = event.y - lastY;

    lastX = event.x;
    lastY = event.y;
    moveCamera(offsetX, offsetY);
}
