#include "game.h"
#include "timestep.h"
#include "input.h"

#include "glm/glm.hpp"

#include "events/eventDispatcher.h"

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

void onEvent(struct Event e)
{
    if (e.type == WindowClose)
        return onWindowClose();
    if (e.type == WindowResize)
        return onWindowResize(e.windowResize);

    if (onEventWorld(e))
        return;

    switch (e.type)
    {
        case KeyPressed:    return onKeyPressed(e.key);
        case KeyReleased:   return;
        case MouseScrolled: return onMouseScrolled(e.mouseScrolled);
        case MouseMoved:    return onMouseMoved(e.mouseMoved);

        default:
            LOG_INFO("Event type not currently handled");
            return;
    }
}

void setUpGame(int width, int height, const char* title)
{
    LOG_INFO_DEBUG("DEBUG");
    createWindow(width, height, title);
    Renderer::Init();
    initECS();

    LOG_TRACE("Initializing client World");
    initWorld();

    running = 1;
    LOG_TRACE("All done!");
}

void onUpdate(double ts)
{
    if (1/ts < 50)
        LOG_WARN("FPS: %f", 1/ts);
    // LOG_INFO_DEBUG("FPS: %f", 1/ts);

    onUpdateWorld(ts);
}

void onRender()
{
    Renderer::StartFrame();
    onRenderWorld();
    Renderer::EndFrame();
}

void runGame()
{
    while (running)
    {
        double ts = getTimestep();
        onUpdate(ts);
        onRender();
        updateWindow();
    }
}

void destroyGame()
{
    destroyWorld();
    destroyECS();
    Renderer::Destroy();
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
    Renderer::SetViewport(event.width, event.height);
}

void onKeyPressed(KeyEvent event)
{
    if (event.repeat)
        return;
    switch (event.key)
    {
        case KEY_M:
        {
            Renderer::ChangeMode();
            break;
        }
        case KEY_C:
        {
            if (event.mods == 0)
                Renderer::SetShader(basicShader);
            break;
        }
        case KEY_U:
        {
            Renderer::SetShader(uvShader);
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
    if (isMouseButtonPressed(MOUSE_BUTTON_LEFT))
        moveCamera(-event.dx, -event.dy);
}
