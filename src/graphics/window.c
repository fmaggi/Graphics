#include "window.h"

#include "stdlib.h"
#include "log/log.h"
#include "log/assert_g.h"

typedef struct _window
{
    GLFWwindow* g_window;
    int width, height;
    EventDispatchFunc eventCallback;
} Window;

Window* window = NULL;

void _errorCallback(int error, const char* description)
{
    LOG_ERROR("OpenGL Error: {%i}:\n", error);
    LOG("  %s\n", description);
}

void _windowCloseCallback(GLFWwindow* window)
{
    Window* userWindow = (Window*) glfwGetWindowUserPointer(window);
    WindowCloseEvent e;
    EventHolder holder;
    holder.instance = &e;
    holder.type = WindowClose;
    userWindow->eventCallback(&holder);
}

void _windowResizeCallback(GLFWwindow* window, int width, int height)
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
    {
        LOG_WARN("Window already created\n");
        return NULL;
    }

    LOG_TRACE("Initializing GLFW and Glad\n");   

    glfwSetErrorCallback(_errorCallback);

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
        glfwTerminate();
        free(window);
        exit(-1);
    }
    glfwMakeContextCurrent(g_window);

    glfwSetWindowUserPointer(g_window, window);

    glfwSetWindowCloseCallback(g_window, _windowCloseCallback);
    glfwSetWindowSizeCallback(g_window, _windowResizeCallback);
    glfwSetKeyCallback(g_window, _keyCallback);
    glfwSetMouseButtonCallback(g_window, _mouseCallback);

    window->g_window = g_window;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD\n");
        destroyWindow();
        exit(-1);
    }

    glViewport(0, 0, width, height);

    return window;
}

void destroyWindow()
{
    free(window);
}

GLFWwindow* getNativeWindow()
{
    ASSERT(window != NULL, "Window not created\n");
    return window->g_window;
}

void prepareWindow()
{
    glfwSwapBuffers(window->g_window);
    glfwPollEvents();
}