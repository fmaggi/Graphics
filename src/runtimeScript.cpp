#include "runtimeScript.h"

LayerData Layer::s_LayerData;

void Layer::OnAttach(uint32_t width, uint32_t height, const std::string& title)
{
    s_LayerData.t = Texture::Create("test.png");
    orthoCamera({0, 0, 0}, width, height);

    Physics::Init(-500);

    s_LayerData.body = Physics::CreateBody({0,300,0}, BodyType::Dynamic);
    Physics::AddAABB(s_LayerData.body, 50, 50);

    Body* floor = Physics::CreateBody({0, -300, 0}, BodyType::Static);
    Physics::AddAABB(floor, 400, 25);

    EventHandler<KeyPressed>::RegisterOnEventFunction(Layer::OnEvent<KeyPressed>);
    EventHandler<MouseMoved>::RegisterOnEventFunction(Layer::OnEvent<MouseMoved>);
}

void Layer::OnUpdate(float ts)
{
    Physics::Step(ts);
}

void Layer::OnRender()
{
    Renderer::PushQuad(s_LayerData.body->translation, 0, {100, 100}, {0.86, 0.3, 0.2}, s_LayerData.t);
    Renderer::PushQuad({0, -300, 0}, 0, {800, 50}, {0.3, 0.6, 0.8}, NoTexture);
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

template<>
bool Layer::OnEvent<MouseMoved>(MouseMoved event)
{
    if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        moveCamera(-event.dx, -event.dy);
        return true;
    }
    return false;
}

void Layer::OnDetach()
{
    EventHandler<KeyPressed>::RemoveOnEventFunction(Layer::OnEvent<KeyPressed>);
}
