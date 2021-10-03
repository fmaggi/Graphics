#include "game.h"

#include "graphics/window.h"
#include "input.h"
#include "graphics/gfx.h"

#include "stdio.h"

typedef struct _game
{
    Window* window;
    struct Keyboard keys;
    struct Mouse mouse;
    int running;
} Game;

static Game game;
static int mode = 0;

Window* getWindow()
{
    return game.window;
}

int keyPressedEvent(KeyEvent* event)
{
    if (event->repeat)
        return 1;
    if (event->key == GLFW_KEY_M)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL * mode + GL_LINE * (1-mode));
        mode = !mode;
    }
    return 1;
}

int eventDispatcher(EventHolder* event)
{
    switch (event->type)
    {
    case KeyPressed:
        keyPressedEvent((KeyEvent*) event->instance);
        break;
    default:
        break;
    }
    return 0;
}

int setUpGame()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    game.window = createWindow(800, 600, "LearnOpenGL", eventDispatcher);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }
    game.running = 1;
}

void runGame()
{

}

void destroyGame()
{

}