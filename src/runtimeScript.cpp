#include "runtimeScript.h"

LayerData Layer::s_LayerData;

void Layer::OnAttach(uint32_t width, uint32_t height, const std::string& title)
{
    s_LayerData.t = Texture::Create("test.png");
    orthoCamera({0, 0, 0}, width, height);

    EventHandler<KeyPressed>::RegisterOnEventFunction(Layer::OnEvent<KeyPressed>);
}

void Layer::OnUpdate(float ts)
{
    if (Input::isKeyPressed(KEY_A))
        s_LayerData.pos.x -= 10;
    else if (Input::isKeyPressed(KEY_D))
        s_LayerData.pos.x += 10;

    if (Input::isKeyPressed(KEY_S))
        s_LayerData.pos.y -= 10;
    else if (Input::isKeyPressed(KEY_W))
        s_LayerData.pos.y += 10;
}

void Layer::OnRender()
{
    Renderer::PushQuad(s_LayerData.pos, 0, {100, 100}, {0.86, 0.3, 0.2}, s_LayerData.t);
    Renderer::PushQuad({150, 0, 0}, 0, {100, 100}, {0.3, 0.6, 0.8}, s_LayerData.t);
    Renderer::PushQuad({600, 0, 0}, 0, {100, 100}, {0.3, 0.6, 0.8}, s_LayerData.t);
    Renderer::PushQuad({-100, 0, 0}, 0, {100, 100}, {0.3, 0.6, 0.8}, s_LayerData.t);
}

template<>
bool Layer::OnEvent<KeyPressed>(KeyPressed event)
{
    switch (event.key)
    {
        case KEY_C:
            if (event.mods & MOD_CONTROL)
            {
                EventHandler<WindowClose>::Dispatch({});
                return true;
            }

        case KEY_U:
            Renderer::SetShader(uvShader);
            return true;

        default:
            return false;
    }
}

void Layer::OnDetach()
{
    EventHandler<KeyPressed>::RemoveOnEventFunction(Layer::OnEvent<KeyPressed>);
}
