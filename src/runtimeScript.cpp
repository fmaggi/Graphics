#include "mainLayer.h"
#include "engine.h"

TextureID t;
glm::vec3 pos{0,0,0};

void Test::OnAttach()
{
    // World creation here
    t = Texture::CreateTexture();
    orthoCamera({0, 0, 0}, m_width, m_height);

    EventHandler<KeyPressed>::RegisterOnEventFunction([](KeyPressed& event){
        if (event.key == KEY_C && event.mods == MOD_CONTROL)
        {
            WindowClose e;
            EventHandler<WindowClose>::Dispatch(e);
            return true;
        }
        return false;
    });
}

void Test::OnUpdate(float ts)
{
    if (Input::isKeyPressed(KEY_A))
        pos.x -= 10;
    else if (Input::isKeyPressed(KEY_D))
        pos.x += 10;

    if (Input::isKeyPressed(KEY_S))
        pos.y -= 10;
    else if (Input::isKeyPressed(KEY_W))
        pos.y += 10;
}

void Test::OnRender()
{
    Renderer::PushQuad(pos, 0, {200, 200}, {0.86, 0.3, 0.2}, t);
}

void Test::OnDetach()
{

}
