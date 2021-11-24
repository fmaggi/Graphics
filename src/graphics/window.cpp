#include "window.h"

#include "gfx.h"

#include "stdlib.h"
#include "log/log.h"

#include "events/eventDispatcher.h"

struct WindowInternal
{
    GLFWwindow* g_window;
    bool created;
    uint32_t width, height;
};

static WindowInternal window = {0, 0};

void errorCallback(int error, const char* description)
{
    LOG_ERROR("OpenGL Error: {%i}:", error);
    LOG("  %s", description);
}

void windowCloseCallback(GLFWwindow* window)
{
    EventHandler<WindowClose>::Dispatch({});
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    WindowResize e(width, height);
    EventHandler<WindowResize>::Dispatch(e);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (action)
    {
        case GLFW_PRESS:
        {
            KeyPressed e(key, scancode, mods, false);
            EventHandler<KeyPressed>::Dispatch(e);
            return;
        }
        case GLFW_RELEASE:
        {
            KeyReleased e(key, scancode, mods);
            EventHandler<KeyReleased>::Dispatch(e);
            return;
        }
        case GLFW_REPEAT:
        {
            KeyPressed e(key, scancode, mods, true);
            EventHandler<KeyPressed>::Dispatch(e);
            return;
        }
    }
}

void mouseMovedCallback(GLFWwindow* window, double x, double y)
{
    static float lastX = 0;
    static float lastY = 0;

    float offsetX = x - lastX;
    float offsetY = y - lastY;

    lastX = x;
    lastY = y;

    MouseMoved e(offsetX, -offsetY);
    EventHandler<MouseMoved>::Dispatch(e);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    MouseScrolled e(xoffset, yoffset);
    EventHandler<MouseScrolled>::Dispatch(e);
}

void Window::Create(uint32_t width, uint32_t height, const std::string& title)
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

    glfwSetWindowUserPointer(g_window, &window);

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

void Window::Destroy()
{
    glfwTerminate();
}

bool Window::IsKeyPressed(int key)
{
    return glfwGetKey(window.g_window, key) == GLFW_PRESS;
}

bool Window::IsMouseButtonPressed(int button)
{
    return glfwGetMouseButton(window.g_window, button) == GLFW_PRESS;
}

void Window::Update()
{
    glfwSwapBuffers(window.g_window);
    glfwPollEvents();
}

void Window::GetCursorPos(double* x, double* y)
{
    // Origin is at the center
    glfwGetCursorPos(window.g_window, x, y);
    *y = window.height/2 - *y;
    *x = -window.width/2 + *x;
}
