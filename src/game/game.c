#include "game.h"

#include "cglm/struct.h"

#include "events/event.h"

#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/gfx.h"

#include "world/world.h"

#include "input/input.h"

#include "log/log.h"

typedef struct _game
{
    int running;
    double lastFrame;
    Window* window;
    Renderer* r;
    World* world;
} Game; //state of the game

static Game game;

void onWindowClose()
{
    game.running = 0;
}

void onWindowResize()
{
    mat4s proj = getProjectionMatrix();
    rendererSetProjectionMatrix(game.r ,proj);
}

void onKeyPressed(KeyEvent event)
{
    if (event.repeat)
        return;
    switch (event.key)
    {
        case GLFW_KEY_M:
        {
            rendererChangeMode();
            break;
        }
        case GLFW_KEY_C:
        {
            if (event.mods == GLFW_MOD_CONTROL)
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
        case WindowResize: return onWindowResize();
        case KeyPressed:   return onKeyPressed(*(KeyEvent*) event->instance);
        case KeyReleased:  return;

        default:
            LOG_INFO("Event type not currently handled\n");           
            return;
    }
}

void setUpGame()
{
    PROFILE_FUNC();
    LOG_INFO_DEBUG("DEBUG\n");
    game.window = createWindow(800, 600, "LearnOpenGL", &onEvent);
    mat4s proj = getProjectionMatrix();

    game.r = createRenderer(proj);

    initInput();

    game.world = emptyWorld();
    initWorld(game.world);

    game.running = 1;
    game.lastFrame = 0;
    LOG_TRACE("All done!\n");
}

void onUpdate()
{

    prepareRenderer();

    double now = glfwGetTime();
    double ts = now - game.lastFrame;
    game.lastFrame = now;
    LOG_INFO_DEBUG("Frametime: %fms\n", ts);

    handleInput(&(game.world->player), ts);
}

void onRender()
{
    render(game.r, game.world);
    prepareWindow(); 
}

void runGame()
{
    while (game.running)
    {
        onUpdate();
        onRender();
    }
}

void destroyGame()
{  
    //glfwTerminate(); 
    destroyWorld(game.world);
    destroyRenderer(game.r);
    destroyWindow();
    LOG_TRACE("Good bye\n");
    PROFILE_FUNC();
}