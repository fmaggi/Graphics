#include "window.h"

#include "gfx.h"

#include "stdlib.h"
#include "log/log.h"
#include "log/assert_g.h"

typedef struct window
{
    GLFWwindow* g_window;
    int width, height;
    EventDispatchFunc eventCallback;
} Window;

Window* window = NULL;

void errorCallback(int error, const char* description)
{
    LOG_ERROR("OpenGL Error: {%i}:\n", error);
    LOG("  %s\n", description);
}

void windowCloseCallback(GLFWwindow* window)
{
    Window* userWindow = (Window*) glfwGetWindowUserPointer(window);
    WindowCloseEvent e;
    EventHolder holder;
    holder.instance = &e;
    holder.type = WindowClose;
    userWindow->eventCallback(&holder);
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Window* userWindow = (Window*) glfwGetWindowUserPointer(window);

    WindowResizeEvent e;
    e.width = width;
    e.height = height;
    EventHolder holder;
    holder.instance = &e;
    holder.type = WindowResize;
    userWindow->eventCallback(&holder);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* userWindow = (Window*) glfwGetWindowUserPointer(window);
    KeyEvent e;
    e.key = key;
    e.scancode = scancode;
    e.action = action;
    e.mods = mods;
    e.repeat = action == GLFW_REPEAT ? 1 : 0;
    EventHolder holder;
    holder.instance = &e;
    holder.type = action == GLFW_RELEASE ? KeyReleased : KeyPressed;
    userWindow->eventCallback(&holder);
}

void mouseMovedCallback(GLFWwindow* window, double x, double y)
{
    Window* userWindow = (Window*) glfwGetWindowUserPointer(window);
    MouseMovedEvent e;
    e.x = (float) x;
    e.y = (float) y;
    EventHolder holder;
    holder.instance = &e;
    holder.type = MouseMoved;
    userWindow->eventCallback(&holder);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* userWindow = (Window*) glfwGetWindowUserPointer(window);
    MouseScrollEvent e;
    e.xoffset = (float) xoffset;
    e.yoffset = (float) yoffset;
    EventHolder holder;
    holder.instance = &e;
    holder.type = MouseScrolled;
    userWindow->eventCallback(&holder);
}

void createWindow(int width, int height, const char* title, EventDispatchFunc callbackFunc)
{
    if (window != NULL)
    {
        LOG_WARN("Window already created\n");
        return;
    }

    LOG_TRACE("Initializing GLFW and Glad\n");   

    glfwSetErrorCallback(errorCallback);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
    window = malloc(sizeof(Window));
    if (window == NULL)
    {
        LOG_ERROR("Failed to create Game Window");
        exit(-1);
    }
    window->eventCallback = callbackFunc;
    window->width  = width;
    window->height = height;

    GLFWwindow* g_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (g_window == NULL)
    {
        LOG_ERROR("Failed to create GLFW window\n");
        free(window);
        exit(-1);
    }
    glfwMakeContextCurrent(g_window);

    glfwSetWindowUserPointer(g_window, window);

    glfwSetWindowCloseCallback(g_window, windowCloseCallback);
    glfwSetWindowSizeCallback(g_window, windowResizeCallback);
    glfwSetKeyCallback(g_window, keyCallback);
    glfwSetScrollCallback(g_window, scrollCallback);
    glfwSetCursorPosCallback(g_window, mouseMovedCallback);

    window->g_window = g_window;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD\n");
        destroyWindow();
        exit(-1);
    }

    glViewport(0, 0, width, height);
}

void destroyWindow()
{
    glfwTerminate();
    free(window);
}

int windowIsKeyPressed(int key)
{
    return glfwGetKey(window->g_window, key) == GLFW_PRESS;
}

int windowIsMouseButtonPressed(int button)
{
    return glfwGetMouseButton(window->g_window, button) == GLFW_PRESS;
}

void updateWindow()
{
    glfwSwapBuffers(window->g_window);
    glfwPollEvents();
}