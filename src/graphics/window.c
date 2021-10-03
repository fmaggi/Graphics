#include "window.h"

#include "gfx.h"

#include "stdlib.h"
#include "stdio.h"


typedef struct _window
{
    GLFWwindow* g_window;
    int width, height;
    EventDispatchFunc eventCallback;
} Window;

Window* window = NULL;

void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* userWindow = (Window*) glfwGetWindowUserPointer(window);
    KeyEvent e = createKeyEvent(key, scancode, action, mods);
    EventHolder holder;
    holder.instance = &e;
    holder.type = action == GLFW_RELEASE ? KeyReleased : KeyPressed;
    userWindow->eventCallback(&holder);
}

void _mouseCallback(GLFWwindow* window, int button, int action, int mods)
{

}

Window* createWindow(int width, int height, const char* title, EventDispatchFunc callbackFunc)
{
    if (window != NULL)
        return window;
        
    window = malloc(sizeof(Window));
    if (window == NULL)
    {
        perror("Failed to create Game Window");
        exit(1);
    }
    window->eventCallback = callbackFunc;
    window->width  = width;
    window->height = height;
    GLFWwindow* g_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (g_window == NULL)
    {
        perror("Failed to create GLFW window");
        glfwTerminate();
        exit(1);
    }
    glfwSetWindowUserPointer(g_window, window);

    glfwSetKeyCallback(g_window, _keyCallback);
    glfwSetMouseButtonCallback(g_window, _mouseCallback);

    window->g_window = g_window;
    glfwMakeContextCurrent(g_window);
    return window;
}

void destroyWindow(Window* window)
{
    free(window);
}

int shouldClose(Window* window)
{
    return glfwWindowShouldClose(window->g_window);
}

void prepare(Window* window)
{
    glfwSwapBuffers(window->g_window);
    glfwPollEvents();
}