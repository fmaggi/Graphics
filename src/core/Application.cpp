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

#include "ImGui/imguiLayer.h"
#include "ImGui/ui.h"

#include "log/log.h"
#include "log/timer.h"

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

    Window::Create(width, height, name, &app->eventSystem);
    Renderer::Init();

    app->isRunning = true;

    app->eventSystem.RegisterListener<&Application::OnWindowClose>(app);
    app->eventSystem.RegisterListener<&Application::OnWindowResize>(app);

    ImGuiLayer::Init(width, height, &app->eventSystem);

    return app;
}

void Application::OnUpdate(float ts)
{
    for (auto m : m_modules)
       m->OnUpdate(ts);
}

void Application::OnRender()
{
    Renderer::SetClearColor(0.2, 0.2, 0.2, 1.0f);
    Renderer::PrepareRenderer();

    for (auto m : m_modules)
       m->OnRender();

    ImGuiLayer::Begin();

    for (auto m : m_modules)
       m->OnRenderUI();

    UI::BeginWindow("Stats");

    ImGui::Text("Frame Time: %.3f ms", ts.dt*1000);

    UI::EndWindow();

    ImGuiLayer::End();
}

void Application::Run()
{
    ASSERT(m_modules.size() > 0, "Module not setup!");

    while (isRunning)
    {
        ts.Step();
        OnUpdate(ts.dt);
        OnRender();
        Window::Update();
    }
    Destroy();
}

void Application::Destroy()
{
    for (auto m : m_modules)
    {
        m->OnDetach(&eventSystem);
        delete m;
    }

    Renderer::Destroy();
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
