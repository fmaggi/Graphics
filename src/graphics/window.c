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
    dispatchEvent((struct Event){.type = WindowClose});
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    struct Event e;
    e.type = WindowResize;
    e.windowResize.width = width;
    e.windowResize.height = height;
    dispatchEvent(e);

    Window* w = glfwGetWindowUserPointer(window);
    w->width = width;
    w->height = height;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    struct Event e;
    e.key.key = key;
    e.key.scancode = scancode;
    e.key.mods = mods;
    e.key.repeat = action == GLFW_REPEAT;
    e.type = action == GLFW_RELEASE ? KeyReleased : KeyPressed;
    dispatchEvent(e);
}

void mouseMovedCallback(GLFWwindow* window, double x, double y)
{
    static float lastX = 0;
    static float lastY = 0;

    float offsetX = x - lastX;
    float offsetY = y - lastY;

    lastX = x;
    lastY = y;
    struct Event e;

    // this is a bit counterintuitive but Ive done it this way to make dx from left to right positiove and dy from down to up positive
    e.mouseMoved.dx = (float) offsetX;
    e.mouseMoved.dy = (float) -offsetY;
    e.type = MouseMoved;
    dispatchEvent(e);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    struct Event e;
    e.type = MouseScrolled;
    e.mouseScrolled.xoffset = (float) xoffset;
    e.mouseScrolled.yoffset = (float) yoffset;
    dispatchEvent(e);
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
