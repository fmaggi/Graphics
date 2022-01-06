#include "Application.h"
#include "module.h"
#include "timestep.h"
#include "input.h"

#include "glm/glm.hpp"

#include "events/eventSystem.h"

#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/camera.h"

#include "ECS/ECS.h"

#include "ImGui/imguiLayer.h"

#include "log/log.h"
#include "log/timer.h"

basic_event_system* EventSystem::internal::event_system = nullptr;

Application* Application::Create(uint32_t width, uint32_t height, const std::string& name)
{
    if (app)
    {
        LOG_WARN("Application already created!");
        return nullptr;
    }

    LOG_INFO_DEBUG("DEBUG");

    app = new Application;
    app->m_width = width;
    app->m_height = height;
    app->name = name;

    Window::Create(width, height, name);
    Renderer::Init();
    ECS::Init();

    EventSystem::internal::event_system = &app->event_system;

    app->isRunning = true;


    EventSystem::RegisterListener<&Application::OnWindowClose>(app);
    EventSystem::RegisterListener<&Application::OnWindowResize>(app);

    ImGuiLayer::Init(width, height);

    return app;
}

void Application::LoadModule(Module* m)
{
    m_modules.push_back(m);
    m->OnAttach(m_width, m_height);
}

void Application::OnUpdate(float ts)
{
    for (auto m : m_modules)
       m->OnUpdate(ts);
}

void Application::OnRender()
{
    Renderer::PrepareRenderer();
    Renderer::SetClearColor(0.2, 0.2, 0.2, 1.0f);

    for (auto m : m_modules)
       m->OnRender();

    ImGuiLayer::Begin();

    for (auto m : m_modules)
       m->OnRenderUI();

    ImGuiLayer::End();
}

void Application::Run()
{
    ASSERT(m_modules.size() > 0, "Module not setup!");

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
    for (auto m : m_modules)
    {
        m->OnDetach();
        delete m;
    }

    Renderer::Destroy();
    ECS::Destroy();
    Window::Destroy();
    LOG_TRACE("Good bye");
}

bool Application::OnWindowClose(WindowClose event)
{
    isRunning = false;
    return true;
}

bool Application::OnWindowResize(WindowResize event)
{
    m_width = event.width;
    m_height = event.height;
    Renderer::SetViewport(event.width, event.height);
    return false;
}
