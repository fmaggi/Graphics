#include "runtimeScript.h"

#include <sstream>

Body* s_b;

void MyLayer::OnAttach(uint32_t width, uint32_t height, const std::string& title)
{
    m_width = width;
    m_height = height;
    m_title = title;

    TextureID tex = Texture::Create("test.png");
    orthoCamera({0, 0, 0}, width, height);

    Physics::Init(-9.8);

    EntityID e = ECS::CreateEntity();
    player = e;

    TransformComponent& t = ECS::AddComponent<TransformComponent>(e);
    t.translation = {0,5,0};
    t.scale = {1, 1};
    t.rotation = 0;

    SpriteComponent& s = ECS::AddComponent<SpriteComponent>(e);
    s.color = {0.86, 0.3, 0.2, 1.0};
    s.texIndex = tex;

    Body* body = Physics::CreateBody(glm::vec2(t.translation), 10, BodyType::Dynamic);
    s_b = body;
    Physics::AddAABB(body, 0.5f, 0.5f);

    body->velocity = {0, 0};
    body->userFlags = 1;

    PhysicsComponent& p = ECS::AddComponent<PhysicsComponent>(e);
    p.physicsBody = body;

    Body* floor = Physics::CreateBody({0, -3}, 0, BodyType::Static);
    Physics::AddAABB(floor, 10, 0.25f);

    EventSystem<KeyPressed>::RegisterListener(this, &MyLayer::OnEvent<KeyPressed>);
    EventSystem<MouseMoved>::RegisterListener(this, &MyLayer::OnEvent<MouseMoved>);
    EventSystem<MouseScrolled>::RegisterListener(this, &MyLayer::OnEvent<MouseScrolled>);
    EventSystem<WindowResize>::RegisterListener(this, &MyLayer::OnEvent<WindowResize>);

}

void MyLayer::OnUpdate(float ts)
{
    if (Input::IsKeyPressed(KEY_SPACE))
        s_b->force.y += 4000;
    Physics::Step(ts);

    auto view = ECS::View<PhysicsComponent, TransformComponent>();

    for (EntityID e : view)
    {
        auto [pc, tc] = view.Get(e);
        Body* b = (Body*) pc.physicsBody;
        tc.translation = glm::vec3(b->translation, tc.translation.z);
    }
}

void MyLayer::OnRender()
{
    auto view = ECS::View<SpriteComponent, TransformComponent>();

    for (EntityID e : view)
    {
        auto [s, t] = view.Get(e);
        Renderer::PushQuad(t.translation, t.rotation, t.scale, s.color, s.texIndex);
    }

    Renderer::PushQuad({0, -3, 0}, 0, {20, 0.5f}, {0.3, 0.6, 0.8, 1.0}, NoTexture);
}

void MyLayer::OnRenderUI()
{
    static ImVec2 mainMenuSize;

    static EntityID selectedEntity = -1;

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open..", "Ctrl+O")) { ECS::CreateEntity(); }
        if (ImGui::MenuItem("Save", "Ctrl+S"))   {  }
        if (ImGui::MenuItem("Close", "Ctrl+W"))  {  }
        ImGui::EndMenu();
    }

    float spacing = m_width - ImGui::GetFontSize() * 2 * 5;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{spacing, 0});
    ImGui::Spacing();
    if (ImGui::RadioButton("Show UI", renderUI))
        renderUI = !renderUI;
    ImGui::PopStyleVar();

    mainMenuSize = ImGui::GetWindowSize();
    ImGui::EndMainMenuBar();

    if (!renderUI)
        return;

    UI::BeginDockSpaceWindow("Content Manager", ImVec2{370.0f, (float)m_height-mainMenuSize.y+1}, ImVec2{(float)m_width, (float)m_height-mainMenuSize.y+1}, ImVec2{0, mainMenuSize.y-1});

    UI::BeginWindow("Entities");

    auto& es = ECS::AllEntities();

    for (EntityID e : es)
    {
        std::stringstream s;
        s << "Entity " << e;
		bool opened = UI::TreeNode((void*)e, s.str());

		if (ImGui::IsItemClicked())
        {
            selectedEntity = e;
        }

		if (opened)
		{
			ImGui::TreePop();
		}
    }

    UI::EndWindow();

    UI::BeginWindow("Properties");

    if (selectedEntity != -1)
    {
        if (ECS::HasComponent<SpriteComponent>(selectedEntity))
        {
            SpriteComponent& s = ECS::GetComponent<SpriteComponent>(selectedEntity);
            ImGui::ColorEdit4("Color", &s.color.x);
        }
        else
        {
            if (ImGui::Button("Add spriteComponent"))
            {
                SpriteComponent& s = ECS::AddComponent<SpriteComponent>(selectedEntity);
                s.color = {0.8, 0.3, 0.4, 1.0f};
                s.texIndex = NoTexture;
            }
        }

        ImGui::Separator();

        if (ECS::HasComponent<TransformComponent>(selectedEntity))
        {
            TransformComponent& t = ECS::GetComponent<TransformComponent>(selectedEntity);
            ImGui::DragFloat3("translation", &t.translation.x);
            ImGui::DragFloat2("scale", &t.scale.x);
            ImGui::DragFloat("rotation", &t.rotation);
        }
        else
        {
            if (ImGui::Button("Add TransformComponent"))
            {
                TransformComponent& t = ECS::AddComponent<TransformComponent>(selectedEntity);
                t.scale = {50, 50};
                t.translation = {0, 0, 0};
                t.rotation = 0;
            }
        }

        ImGui::Separator();

        if (ImGui::Button("Delete"))
        {
            ECS::DestroyEntity(selectedEntity);
            selectedEntity = -1;
        }

    }

    UI::EndWindow();

    UI::EndDockSpaceWindow();
}

template<>
bool MyLayer::OnEvent<KeyPressed>(KeyPressed event)
{
    switch (event.key)
    {
        case KEY_C:
            if (event.mods & MOD_CONTROL)
            {
                EventSystem<WindowClose>::Emit(WindowClose{});
                return true;
            }
            return false;

        case KEY_U:
            Renderer::SetShader(uvShader);
            return true;

        case KEY_H:
            renderUI = !renderUI;
            return true;

        default:
            return false;
    }
}

template<>
bool MyLayer::OnEvent<MouseMoved>(MouseMoved event)
{
    if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        moveCamera(-event.dx, -event.dy);
        return true;
    }
    return false;
}

template<>
bool MyLayer::OnEvent<MouseScrolled>(MouseScrolled event)
{
    updateZoom(event.dy);
    return true;
}

template<>
bool MyLayer::OnEvent<WindowResize>(WindowResize event)
{
    m_width = event.width;
    m_height = event.height;
    return false;
}

void MyLayer::OnDetach()
{
    EventSystem<KeyPressed>::UnregisterListener(this);
    EventSystem<MouseMoved>::UnregisterListener(this);
    EventSystem<MouseScrolled>::UnregisterListener(this);
    EventSystem<WindowResize>::UnregisterListener(this);
}
