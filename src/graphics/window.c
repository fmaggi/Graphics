#include "window.h"

#include "gfx.h"

#include "stdlib.h"
#include "log/log.h"

#include "events/eventDispatcher.h"

typedef struct window
{
    GLFWwindow* g_window;
    bool created;
    int width, height;
} Window;

static Window window = {0, 0};

void errorCallback(int error, const char* description)
{
    LOG_ERROR("OpenGL Error: {%i}:", error);
    LOG("  %s", description);
}

void windowCloseCallback(GLFWwindow* window)
{
    dispatchEvent(0, WindowClose);
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    WindowResizeEvent e;
    e.width = width;
    e.height = height;
    dispatchEvent(&e, WindowResize);

    Window* w = glfwGetWindowUserPointer(window);
    w->width = width;
    w->height = height;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    KeyEvent e;
    e.key = key;
    e.scancode = scancode;
    e.mods = mods;
    e.repeat = action == GLFW_REPEAT ? 1 : 0;
    enum EventType type = action == GLFW_RELEASE ? KeyReleased : KeyPressed;
    dispatchEvent(&e, type);
}

void mouseMovedCallback(GLFWwindow* window, double x, double y)
{
    MouseMovedEvent e;
    e.x = (float) x;
    e.y = (float) y;
    dispatchEvent(&e, MouseMoved);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    MouseScrollEvent e;
    e.xoffset = (float) xoffset;
    e.yoffset = (float) yoffset;
    dispatchEvent(&e, MouseScrolled);
}

void createWindow(int width, int height, const char* title)
{
    if (window.created)
    {
        LOG_WARN("Window already created");
        return;
    }
    window.created = true;

    LOG_TRACE("Initializing GLFW and Glad");

    glfwSetErrorCallback(errorCallback);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* g_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (g_window == NULL)
    {
        LOG_ERROR("Failed to create GLFW window");
        exit(-1);
    }
    glfwMakeContextCurrent(g_window);

    glfwSetWindowCloseCallback(g_window, windowCloseCallback);
    glfwSetWindowSizeCallback(g_window, windowResizeCallback);
    glfwSetKeyCallback(g_window, keyCallback);
    glfwSetScrollCallback(g_window, scrollCallback);
    glfwSetCursorPosCallback(g_window, mouseMovedCallback);

    glfwSetWindowUserPointer(g_window, &window);

    window.g_window = g_window;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        destroyWindow();
        exit(-1);
    }

    glViewport(0, 0, width, height);

    window.width = width;
    window.height = height;
}

void destroyWindow()
{
    glfwTerminate();
}

int windowIsKeyPressed(int key)
{
    return glfwGetKey(window.g_window, key) == GLFW_PRESS;
}

int windowIsMouseButtonPressed(int button)
{
    return glfwGetMouseButton(window.g_window, button) == GLFW_PRESS;
}

void updateWindow()
{
    glfwSwapBuffers(window.g_window);
    glfwPollEvents();
}

void windowGetCursorPos(double* x, double* y)
{
    // Origin is at the center
    glfwGetCursorPos(window.g_window, x, y);
    *y = window.height/2 - *y;
    *x = -window.width/2 + *x;
}
