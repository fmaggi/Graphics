#include "game.h"
#include "layer.h"
#include "timestep.h"
#include "input.h"

#include "glm/glm.hpp"

#include "events/event.h"
#include "events/eventDispatcher.h"

#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/camera.h"

#include "entity/entity.h"

#include "ImGui/imguiLayer.h"

#include "log/log.h"

static bool running = 0;

bool Game::SetUp(GameDef& def)
{
    LOG_INFO_DEBUG("DEBUG");

    Window::Create(def.width, def.height, def.title);
    Renderer::Init();
    ECS::Init();

    LOG_TRACE("Initializing client World");
    Layer::OnAttach(def.width, def.height, def.title);

    ImGuiLayer::Init((float)def.width, (float)def.height);

    running = 1;
    LOG_TRACE("All done!");

    EventHandler<WindowClose>::RegisterOnEventFunction([](WindowClose event){
        running = 0;
        return true;
    });

    EventHandler<WindowResize>::RegisterOnEventFunction([](WindowResize event){
        Renderer::SetViewport(event.width, event.height);
        updateProjectionMatrix(event.width, event.height);
        return false;
    });

    return 1;
}

void Game::OnUpdate(float ts)
{
    if (1/ts < 50)
        LOG_WARN("FPS: %f", 1/ts);
    // LOG_INFO_DEBUG("FPS: %f", 1/ts);

    Layer::OnUpdate(ts);
}

void Game::OnRender()
{
    Renderer::StartFrame(camera);
    Layer::OnRender();
    Renderer::EndFrame();

    ImGuiLayer::Begin();
    Layer::OnRenderUI();
    ImGuiLayer::End();
}

void Game::Run()
{
    while (running)
    {
        float ts = getTimestep();
        Game::OnUpdate(ts);
        Game::OnRender();
        Window::Update();
    }
    Destroy();
}

void Game::Destroy()
{
    Layer::OnDetach();
    Renderer::Destroy();
    ECS::Destroy();
    Window::Destroy();
    LOG_TRACE("Good bye");
}
