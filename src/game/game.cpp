#include "game.h"
#include "layer.h"
#include "timestep.h"
#include "input.h"

#include "glm/glm.hpp"

#include "events/eventDispatcher.h"

#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/camera.h"

#include "entity/entity.h"

#include "log/log.h"

static bool running = 0;

bool Game::SetUp(uint32_t width, uint32_t height, const std::string& title)
{
    EventHandler<WindowClose>::RegisterOnEventFunction([](WindowClose event){
        running = 0;
        return true;
    });

    LOG_INFO_DEBUG("DEBUG");
    Window::Create(width, height, title.c_str());
    Renderer::Init();
    // initECS();

    LOG_TRACE("Initializing client World");
    Layer::OnAttach(width, height, title);

    running = 1;
    LOG_TRACE("All done!");
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
    // destroyECS();
    Renderer::Destroy();
    Window::Destroy();
    LOG_TRACE("Good bye");
}
