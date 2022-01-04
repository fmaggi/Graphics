#include "window.h"

#include "gfx.h"

#include "stdlib.h"
#include "log/log.h"

#include "events/event.h"
#include "events/eventSystem.h"

namespace Window {

struct WindowInternal
{
    GLFWwindow* g_window;
    bool created;
    uint32_t width, height;
};

static WindowInternal window = {0, 0, 0, 0};

static void errorCallback(int error, const char* description)
{
    LOG_ERROR("OpenGL Error: {%i}:", error);
    LOG("  %s", description);
}

static void windowCloseCallback(GLFWwindow* window)
{
    EventSystem::Emit(WindowClose{});
}

static void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    WindowResize e(width, height);
    EventSystem::Emit(e);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (action)
    {
        case GLFW_PRESS:
        {
            KeyPressed e(key, scancode, mods, false);
            EventSystem::Emit(e);
            return;
        }
        case GLFW_RELEASE:
        {
            KeyReleased e(key, scancode, mods);
            EventSystem::Emit(e);
            return;
        }
        case GLFW_REPEAT:
        {
            KeyPressed e(key, scancode, mods, true);
            EventSystem::Emit(e);
            return;
        }
    }
}

static void mouseMovedCallback(GLFWwindow* window, double x, double y)
{
    static float lastX = 0;
    static float lastY = 0;

    float offsetX = x - lastX;
    float offsetY = y - lastY;

    lastX = x;
    lastY = y;

    MouseMoved e(offsetX, -offsetY);
    EventSystem::Emit(e);
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    MouseScrolled e(xoffset, yoffset);
    EventSystem::Emit(e);
}

static void windowMaximizeCallback(GLFWwindow* window, int maximized)
{
    glfwSetWindowAttrib(window, GLFW_DECORATED, !maximized);
}

void Create(uint32_t width, uint32_t height, const std::string& title, bool vsync)
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* g_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
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

    glfwSetWindowMaximizeCallback(g_window, windowMaximizeCallback);
    glfwSetWindowUserPointer(g_window, &window);

    glfwSwapInterval(vsync);

    window.g_window = g_window;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        exit(-1);
    }

    glViewport(0, 0, width, height);

    window.width = width;
    window.height = height;
}

void Destroy()
{
    glfwTerminate();
}

bool IsKeyPressed(int key)
{
    return glfwGetKey(window.g_window, key) == GLFW_PRESS;
}

bool IsMouseButtonPressed(int button)
{
    return glfwGetMouseButton(window.g_window, button) == GLFW_PRESS;
}

void Update()
{
    glfwSwapBuffers(window.g_window);
    glfwPollEvents();
}

void GetCursorPos(double* x, double* y)
{
    // Origin is at the center
    glfwGetCursorPos(window.g_window, x, y);
    *y = window.height/2 - *y;
    *x = -window.width/2 + *x;
}

void* GetNativeWindow()
{
    return window.g_window;
}

};
