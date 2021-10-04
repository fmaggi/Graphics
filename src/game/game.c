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
    Renderer* renderer;
    World* world;
} Game; //state of the game

static Game game;

void onWindowClose()
{
    game.running = 0;
}

void onWindowResize()
{
    mat4s proj = getProjectionMatrix(game.window);
    rendererSetProjectionMatrix(game.renderer, proj);
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
    game.window = createWindow(800, 600, "LearnOpenGL", &onEvent);
    mat4s proj = getProjectionMatrix(game.window);

    game.renderer = createRenderer(proj);

    initInput(getNativeWindow(game.window));

    game.world = emptyWorld();
    initWorld(game.world);

    game.running = 1;
    game.lastFrame = 0;
    LOG_TRACE("All done!\n");
}

void onUpdate()
{
    prepareRenderer();
    rendererUseShader(game.renderer, basicShader);

    double now = glfwGetTime();
    double ts = now - game.lastFrame;
    game.lastFrame = now;
    // LOG_INFO("Frametime: %fms\n", ts);

    handleInput(&(game.world->entities[0]), ts);
}

void onRender()
{
    render(game.renderer, game.world);

    prepareWindow(game.window); 
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
    destroyWorld(game.world);
    destroyRenderer(game.renderer);
    destroyWindow(game.window);
    glfwTerminate();
    LOG_TRACE("Good bye\n");
}