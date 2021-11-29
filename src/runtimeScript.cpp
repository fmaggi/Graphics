#include "runtimeScript.h"

LayerData Layer::s_LayerData;

bool s_RenderUI = true;

void Layer::OnAttach(uint32_t width, uint32_t height, const std::string& title)
{
    TextureID tex = Texture::Create("test.png");
    orthoCamera({0, 0, 0}, width, height);

    Physics::Init(0);

    EntityID e = ECS::CreateEntity();
    s_LayerData.player = e;

    TransformComponent& t = ECS::AddComponent<TransformComponent>(e);
    t.translation = {0,0,0};
    t.scale = {100, 100};
    t.rotation = 0;

    SpriteComponent& s = ECS::AddComponent<SpriteComponent>(e);
    s.color = {0.86, 0.3, 0.2};
    s.texIndex = tex;

    Body* body = Physics::CreateBody({0, 300, 0}, BodyType::Dynamic);
    Physics::AddAABB(body, 50, 50);

    PhysicsComponent& p = ECS::AddComponent<PhysicsComponent>(e);
    p.physicsBody = body;

    Body* floor = Physics::CreateBody({0, -300, 0}, BodyType::Static);
    Physics::AddAABB(floor, 400, 25);

    EventHandler<KeyPressed>::RegisterOnEventFunction(Layer::OnEvent<KeyPressed>);
    EventHandler<MouseMoved>::RegisterOnEventFunction(Layer::OnEvent<MouseMoved>);
}

void Layer::OnUpdate(float ts)
{
    Physics::Step(ts);

    TransformComponent& t = ECS::GetComponent<TransformComponent>(s_LayerData.player);
    PhysicsComponent& p = ECS::GetComponent<PhysicsComponent>(s_LayerData.player);

    Body* b = (Body*) p.physicsBody;
    t.translation = b->translation;

    auto view = ECS::View<TransformComponent>();

    for (TransformComponent& t : view)
    {
    }
}

void Layer::OnRender()
{
    TransformComponent& t = ECS::GetComponent<TransformComponent>(s_LayerData.player);
    SpriteComponent& s = ECS::GetComponent<SpriteComponent>(s_LayerData.player);

    Renderer::PushQuad(t.translation, t.rotation, t.scale, s.color, s_LayerData.t);
    Renderer::PushQuad({0, -300, 0}, 0, {800, 50}, {0.3, 0.6, 0.8}, NoTexture);
}

void Layer::OnRenderUI()
{
    if (!s_RenderUI)
        return;
    TransformComponent& t = ECS::GetComponent<TransformComponent>(s_LayerData.player);
    PhysicsComponent& p = ECS::GetComponent<PhysicsComponent>(s_LayerData.player);
    Body* b = (Body*)p.physicsBody;

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::Begin("Menu", nullptr, window_flags);

    ImGui::BeginChild("Player Transform");

    for (int i = 0; i < 10; i++)
    {
        std::string l = "Label ";
        l += '0'+ i;
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
        bool open = ImGui::TreeNodeEx((void*)(uint64_t)i, flags, l.c_str());

        if (ImGui::IsItemClicked() && !open)
        {
            LOG_INFO("%i", i);
        }

        if (open)
        {
            ImGui::Text("Hello");
            ImGui::TreePop();
        }
    }

    ImGui::EndChild();

    ImGui::End();
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
            return false;

        case KEY_U:
            Renderer::SetShader(uvShader);
            return true;

        case KEY_H:
            s_RenderUI = !s_RenderUI;
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
    EventHandler<MouseMoved>::RemoveOnEventFunction(Layer::OnEvent<MouseMoved>);
}
