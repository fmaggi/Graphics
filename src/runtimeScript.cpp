#include "mainLayer.h"
#include "engine.h"

Texture* t;

void Test::OnAttach()
{
    // World creation here
    t = new Texture("");
    orthoCamera({0, 0, 0}, m_width, m_height);

    EventHandler<KeyEvent>::RegisterOnEventFunction([](KeyEvent& event){
        if (event.GetType() == EventType::KeyPressed)
            // if (event.key == KEY_C)
            //     EventHandler<WindowClose>::Dispatch();
        return true;
    });
}

void Test::OnUpdate(float ts)
{
    moveCamera(3, 0);
}

void Test::OnRender()
{
    Renderer::PushQuad({0, 0, 0}, 0, {200, 200}, {0.86, 0.3, 0.2}, *t);
}

void Test::OnEvent(Event& event)
{

}

void Test::OnDetach()
{

}
