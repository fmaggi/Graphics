#include "window.h"

#include "gfx.h"

#include "stdlib.h"
#include "log/log.h"

#include "events/eventDispatcher.h"

typedef struct window
{
    GLFWwindow* g_window;
    bool created;
} Window;

static Window window = {0, 0};

void errorCallback(int error, const char* description)
{
    LOG_ERROR("OpenGL Error: {%i}:", error);
    LOG("  %s", description);
}

void windowCloseCallback(GLFWwindow* window)
{
    WindowCloseEvent e;
    EventHolder holder;
    holder.instance = &e;
    holder.type = WindowClose;
    dispatchEvent(&holder);
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    WindowResizeEvent e;
    e.width = width;
    e.height = height;
    EventHolder holder;
    holder.instance = &e;
    holder.type = WindowResize;
    dispatchEvent(&holder);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    KeyEvent e;
    e.key = key;
    e.scancode = scancode;
    e.mods = mods;
    e.repeat = action == GLFW_REPEAT ? 1 : 0;
    EventHolder holder;
    holder.instance = &e;
    holder.type = action == GLFW_RELEASE ? KeyReleased : KeyPressed;
    dispatchEvent(&holder);
}

void mouseMovedCallback(GLFWwindow* window, double x, double y)
{
    MouseMovedEvent e;
    e.x = (float) x;
    e.y = (float) y;
    EventHolder holder;
    holder.instance = &e;
    holder.type = MouseMoved;
    dispatchEvent(&holder);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    MouseScrollEvent e;
    e.xoffset = (float) xoffset;
    e.yoffset = (float) yoffset;
    EventHolder holder;
    holder.instance = &e;
    holder.type = MouseScrolled;
    dispatchEvent(&holder);
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

    glfwSetWindowUserPointer(g_window, &window);

    glfwSetWindowCloseCallback(g_window, windowCloseCallback);
    glfwSetWindowSizeCallback(g_window, windowResizeCallback);
    glfwSetKeyCallback(g_window, keyCallback);
    glfwSetScrollCallback(g_window, scrollCallback);
    glfwSetCursorPosCallback(g_window, mouseMovedCallback);

    window.g_window = g_window;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        destroyWindow();
        exit(-1);
    }

    glViewport(0, 0, width, height);
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
