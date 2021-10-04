#include "game.h"

#include "events/event.h"

#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/gfx.h"

#include "world/world.h"

#include "log/log.h"

typedef struct _game
{
    int running;
    Window* window;
    Renderer* renderer;
    World* world;
} Game;

static Game game;

void onWindowClose()
{
    game.running = 0;
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
        case WindowClose: return onWindowClose();
        case KeyPressed:  return onKeyPressed(*(KeyEvent*) event->instance);
        default:          return;
    }
}

void setUpGame()
{
    game.window = createWindow(800, 600, "LearnOpenGL", onEvent);
    game.renderer = createRenderer();

    game.world = emptyWorld();

    initWorld(game.world);

    game.running = 1;
    LOG_TRACE("All done!\n");
}

void runGame()
{
    while (game.running)
    {
        prepareRenderer();
        rendererUseShader(game.renderer, basicShader);

        render(game.world);

        prepareWindow(game.window);
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