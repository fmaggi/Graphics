#include "game.h"
#include "timestep.h"
#include "input.h"

#include "glm/glm.hpp"

#include "events/eventDispatcher.h"

#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/camera.h"

#include "world.h"
#include "entity/entity.h"

#include "log/log.h"

static Layer* s_layer = 0;
static bool running = 0;

void Game::OnEvent(Event& event)
{
    s_layer->OnEvent(event);
}

bool Game::SetUp(Layer* layer)
{
    EventHandler<WindowClose>::RegisterOnEventFunction([](WindowClose& event){
        running = 0;
        return true;;
    });

    s_layer = layer;
    LOG_INFO_DEBUG("DEBUG");
    Window::Create(layer->m_width, layer->m_height, layer->m_title.c_str());
    Renderer::Init();
    // initECS();

    LOG_TRACE("Initializing client World");
    s_layer->OnAttach();

    running = 1;
    LOG_TRACE("All done!");
    return 1;
}

void Game::OnUpdate(float ts)
{
    if (1/ts < 50)
        LOG_WARN("FPS: %f", 1/ts);
    // LOG_INFO_DEBUG("FPS: %f", 1/ts);

    s_layer->OnUpdate(ts);
}

void Game::OnRender()
{
    Renderer::StartFrame();
    s_layer->OnRender();
    Renderer::EndFrame();
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
}

void Game::Destroy()
{
    s_layer->OnDetach();
    // destroyECS();
    Renderer::Destroy();
    Window::Destroy();
    LOG_TRACE("Good bye");
}
