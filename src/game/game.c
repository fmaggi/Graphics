#include "game.h"
#include "timestep.h"

#include "cglm/struct.h"

#include "events/event.h"

#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/camera.h"

#include "world/world.h"

#include "input/input.h"

#include "log/log.h"

typedef struct _game
{
    int running;
    World* world;
    Camera camera;
} Game; //state of the game

static Game game;

void onWindowClose()
{
    game.running = 0;
}

void onWindowResize(WindowResizeEvent event)
{
    updateProjectionMatrix(&game.camera, event.width, event.height);
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
                game.running = 0;
            break;
        }
        default:
        {
            break;
        }
    }
}

void onEvent(EventHolder* event)
{
    switch (event->type)
    {   
        case WindowClose:  return onWindowClose();
        case WindowResize: return onWindowResize(*(WindowResizeEvent*) event->instance);
        case KeyPressed:   return onKeyPressed(*(KeyEvent*) event->instance);
        case KeyReleased:  return;

        default:
            LOG_INFO("Event type not currently handled\n");           
            return;
    }
}

void setUpGame()
{
    LOG_INFO_DEBUG("DEBUG\n");
    createWindow(800, 600, "LearnOpenGL", &onEvent);
    game.camera = orthoCamera((vec3s){0, 0, -1}, 800, 600);

    createRenderer();

    initInput();

    game.world = emptyWorld();
    initWorld(game.world);

    game.running = 1;
    LOG_TRACE("All done!\n");
}

void onUpdate()
{
    double ts = getTimestep();
    LOG_INFO_DEBUG("Frametime: %fms\n", ts);

    handleInput(&(game.camera), ts);
}

void onRender()
{
    startFrame(&game.camera);
    render(game.world);
    endFrame();
}

void runGame()
{
    while (game.running)
    {
        onUpdate();
        onRender();
        updateWindow();
    }
}

void destroyGame()
{  
    destroyWorld(game.world);
    destroyRenderer();
    destroyWindow();
    LOG_TRACE("Good bye\n");
}