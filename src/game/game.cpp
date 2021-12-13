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

#include "entity/ECS.h"

#include "ImGui/imguiLayer.h"

#include "log/log.h"
#include "log/timer.h"

namespace Game {

static bool running = 0;
static Layer* s_baseLayer;

bool SetUp(GameDef def)
{
    LOG_INFO_DEBUG("DEBUG");

    s_baseLayer = def.baseLayer;

    Window::Create(def.width, def.height, def.title);
    Renderer::Init();
    ECS::Init();

    LOG_TRACE("Initializing client World");
    s_baseLayer->OnAttach(def.width, def.height, def.title);

    ImGuiLayer::Init((float)def.width, (float)def.height);

    running = 1;
    LOG_TRACE("All done!");

    EventHandler<WindowClose>::RegisterOnEventFunction([&](WindowClose event){
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

void OnUpdate(float ts)
{
   s_baseLayer->OnUpdate(ts);
}

void OnRender()
{
    Renderer::StartFrame(camera);
    s_baseLayer->OnRender();
    Renderer::EndFrame();

    ImGuiLayer::Begin();
    s_baseLayer->OnRenderUI();
    ImGuiLayer::End();
}

void Run()
{
    while (running)
    {
        float ts = getTimestep();
        OnUpdate(ts);
        OnRender();
        Window::Update();
    }
    Destroy();
}

void Destroy()
{
    s_baseLayer->OnDetach();
    Renderer::Destroy();
    ECS::Destroy();
    Window::Destroy();
    LOG_TRACE("Good bye");
}

};
