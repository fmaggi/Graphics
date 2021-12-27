#include "Application.h"
#include "module.h"
#include "timestep.h"
#include "input.h"

#include "glm/glm.hpp"

#include "events/event.h"
#include "events/eventSystem.h"

#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/camera.h"

#include "entity/ECS.h"

#include "ImGui/imguiLayer.h"

#include "log/log.h"
#include "log/timer.h"

Application* Application::Create(uint32_t width, uint32_t height, const std::string& name)
{
    static bool created = false;
    if (created)
    {
        LOG_WARN("Application already created!");
        return nullptr;
    }

    LOG_INFO_DEBUG("DEBUG");

    Application* app = new Application;
    app->m_width = width;
    app->m_height = height;
    app->name = name;

    Window::Create(width, height, name);
    Renderer::Init();
    ECS::Init();

    app->isRunning = 1;
    LOG_TRACE("All done!");

    EventSystem<WindowClose>::RegisterFunction([app](WindowClose event){
        app->isRunning = 0;
        return true;
    });

    EventSystem<WindowResize>::RegisterFunction([](WindowResize event){
        Renderer::SetViewport(event.width, event.height);
        return false;
    });

    return app;
}

void Application::LoadModule(Module* module)
{
    mainModule = module;
    mainModule->OnAttach(m_width, m_height, name);

    ImGuiLayer::Init(m_width, m_height);
}

void Application::OnUpdate(float ts)
{
   mainModule->OnUpdate(ts);
}

void Application::OnRender()
{
    mainModule->OnRender();

    ImGuiLayer::Begin();
    mainModule->OnRenderUI();
    ImGuiLayer::End();
}

void Application::Run()
{
    ASSERT(mainModule, "Module not setup!");

    while (isRunning)
    {
        float ts = getTimestep();
        OnUpdate(ts);
        OnRender();
        Window::Update();
    }
    Destroy();
}

void Application::Destroy()
{
    mainModule->OnDetach();
    Renderer::Destroy();
    ECS::Destroy();
    Window::Destroy();
    LOG_TRACE("Good bye");
}
