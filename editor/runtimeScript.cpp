#include "runtimeScript.h"

#include <sstream>

Body* s_b;

void MyLayer::OnAttach(uint32_t width, uint32_t height, EventSystem* eventSystem_)
{
    renderUI = false;
    m_width = width;
    m_height = height;
    m_title = "Editor";
    eventSystem = eventSystem_;

    camera.SetViewport(width, height);

    TextureID tex = Texture::Create("test.png");

    srand(time(nullptr));

    world.gravity = { 0, -9.8 };
    {
        EntityID e = ECS::CreateEntity();
        player = e;

        TransformComponent& t = ECS::AddComponent<TransformComponent>(e);
        t.translation = {0,0,0};
        t.scale = {0.5, 0.5};
        t.rotation = 0;

        SpriteComponent& s = ECS::AddComponent<SpriteComponent>(e);
        s.color = {0.86, 0.3, 0.2, 1.0};
        s.texIndex = tex;

        Body* body = world.CreateBody(glm::vec2(t.translation), 10, BodyType::Dynamic, 0, 0, 2);
        s_b = body;
        world.AddAABB(body, 0.25, 0.25);
        body->force = { 150, -300 };

        PhysicsComponent& p = ECS::AddComponent<PhysicsComponent>(e);
        p.physicsBody = body;
    }

        Body* floor = world.CreateBody({0, -3.5}, 0, BodyType::Static);
        world.AddAABB(floor, 6.75, 0.25f);

        Body* roof = world.CreateBody({0, 3.5}, 0, BodyType::Static);
        world.AddAABB(roof, 6.75, 0.25f);

        Body* left = world.CreateBody({-6.5, 0}, 0, BodyType::Static);
        world.AddAABB(left, 0.25, 3.5);

        Body* right = world.CreateBody({6.5, 0}, 0, BodyType::Static);
        world.AddAABB(right, 0.25, 3.5);

    for (int i = 0; i < 5; i++)
    {
        EntityID id = ECS::CreateEntity();

        TransformComponent& ts = ECS::AddComponent<TransformComponent>(id);
        ts.translation = {-3 + i * 2,0,0};
        log_vec3("", ts.translation);
        ts.scale = {0.5, 0.5};
        ts.rotation = 0;

        SpriteComponent& ss = ECS::AddComponent<SpriteComponent>(id);
        ss.color = {(float)(rand() % 10) / 10.0f, (float)(rand() % 10) / 10.0f, (float)(rand() % 10) / 10.0f, 1.0};
        ss.texIndex = tex;

        Body* body = world.CreateBody(glm::vec2(ts.translation), 10, BodyType::Dynamic, 0, 0, 1);
        world.AddAABB(body, 0.25, 0.25);

        PhysicsComponent& p = ECS::AddComponent<PhysicsComponent>(id);
        p.physicsBody = body;
        LOG_WARN("%p", s_b);
    }

    eventSystem->RegisterListener<&MyLayer::OnEvent<KeyPressed>>(this);
    eventSystem->RegisterListener<&MyLayer::OnEvent<MouseMoved>>(this);
    eventSystem->RegisterListener<&MyLayer::OnEvent<MouseScrolled>>(this);
    eventSystem->RegisterListener<&MyLayer::OnEvent<WindowResize>>(this);
}

void MyLayer::OnUpdate(float ts)
{
    if (Input::IsKeyPressed(KEY_SPACE))
        s_b->force.y += 300;

    if (Input::IsKeyPressed(KEY_A))
        s_b->force.x -= 20;
    else if (Input::IsKeyPressed(KEY_D))
        s_b->force.x += 20;

    if (!pause)
    {
        float dt = 1 / 120.0f;
        world.Step(dt);
    }

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
    Renderer::StartFrame(camera);
    {
        auto view = ECS::View<SpriteComponent, TransformComponent>();

        for (EntityID e : view)
        {
            auto [s, t] = view.Get(e);
            Renderer::PushQuad(t.translation, t.rotation, t.scale, s.color, s.texIndex);
        }

        Renderer::PushQuad({0, -3.5, 0}, 0, {12.5, 0.5f}, {0.3, 0.6, 0.8, 1.0}, NoTexture);
        Renderer::PushQuad({-6.5, 0, 0}, 0, {0.5f, 7}, {0.3, 0.6, 0.8, 1.0}, NoTexture);
        Renderer::PushQuad({0,  3.5, 0}, 0, {12.5, 0.5f}, {0.3, 0.6, 0.8, 1.0}, NoTexture);
        Renderer::PushQuad({ 6.5, 0, 0}, 0, {0.5f, 7}, {0.3, 0.6, 0.8, 1.0}, NoTexture);
    }
    Renderer::EndFrame();
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
		bool opened = UI::TreeNode((void*)(uint64_t)e, s.str());

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
                eventSystem->Emit(WindowClose{});
                return true;
            }
            return false;

        case KEY_U:
            Renderer::SetShader(uvShader);
            return true;

        case KEY_H:
            renderUI = !renderUI;
            return true;

        case KEY_P:
            pause = !pause;
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
        camera.Move({-event.dx, -event.dy});
        return true;
    }
    return false;
}

template<>
bool MyLayer::OnEvent<MouseScrolled>(MouseScrolled event)
{
    camera.Zoom(event.dy);
    return true;
}

template<>
bool MyLayer::OnEvent<WindowResize>(WindowResize event)
{
    m_width = event.width;
    m_height = event.height;
    camera.SetViewport(event.width, event.height);
    return false;
}

void MyLayer::OnDetach(EventSystem* eventSystem)
{
    LOG_WARN("%p", s_b);
}
