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

    app->isRunning = true;
    EventSystem::internal::event_system = new basic_event_system;

    EventSystem::RegisterListener<&Application::OnWindowClose>(app);
    EventSystem::RegisterListener<&Application::OnWindowResize>(app);

    ImGuiLayer::Init(width, height);

    return app;
}

void Application::LoadModule(Module* module)
{
    m_modules.push_back(module);
    module->OnAttach(m_width, m_height);
}

void Application::OnUpdate(float ts)
{
    for (auto module : m_modules)
       module->OnUpdate(ts);
}

void Application::OnRender()
{
    Renderer::SetClearColor(0.2, 0.2, 0.2, 1.0f);
    Renderer::PrepareRenderer();

    for (auto module : m_modules)
       module->OnRender();

    ImGuiLayer::Begin();

    for (auto module : m_modules)
       module->OnRenderUI();

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
    for (auto module : m_modules)
       module->OnDetach();

    Renderer::Destroy();
    ECS::Destroy();
    Window::Destroy();
    delete EventSystem::internal::event_system;
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
