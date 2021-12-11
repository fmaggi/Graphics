#include "imguiLayer.h"

#include "events/eventDispatcher.h"
#include "graphics/window.h"

#include <imgui.h>

#include "ui.h"

#define IMGImGuiLayer_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

static float s_width = 0, s_height = 0;
static void SetDarkThemeColors();

template<typename E>
static bool KeyboardCallback(E event)
{
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureKeyboard;
}

template<typename E>
static bool MouseCallback(E event)
{
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

void ImGuiLayer::Init(float width, float height)
{
    LOG_TRACE("Initializing ImGui");

    s_width = width;
    s_height = height;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    float fontSize = 22.0f;// *2.0f;
	UI::FontBold = io.Fonts->AddFontFromFileTTF("res/fonts/firacode/FiraCode-Bold.ttf", fontSize);
	UI::FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/firacode/FiraCode-Regular.ttf", fontSize);
    io.FontDefault = UI::FontDefault;

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;


	// Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    SetDarkThemeColors();

    // Setup Platform/Renderer backends
    GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetNativeWindow());
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    EventHandler<WindowResize>::RegisterOnEventFunction([](WindowResize event){
        s_width = event.width;
        s_height = event.height;
        return false;
    });

    EventHandler<KeyPressed>::RegisterOnEventFunction(KeyboardCallback<KeyPressed>);
    EventHandler<KeyReleased>::RegisterOnEventFunction(KeyboardCallback<KeyReleased>);
    EventHandler<MouseButtonPressed>::RegisterOnEventFunction(MouseCallback<MouseButtonPressed>);
    EventHandler<MouseButtonReleased>::RegisterOnEventFunction(MouseCallback<MouseButtonReleased>);
    EventHandler<MouseMoved>::RegisterOnEventFunction(MouseCallback<MouseMoved>);
    EventHandler<MouseScrolled>::RegisterOnEventFunction(MouseCallback<MouseScrolled>);
}

void ImGuiLayer::Destroy()
{

}

void ImGuiLayer::Begin()
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(s_width, s_height);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void SetDarkThemeColors()
{
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.26f, 0.6f, 0.729f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.20f, 0.46f, 0.56f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.10f, 0.1005f, 0.101f, 1.0f };

    colors[ImGuiCol_Border] = ImVec4{ 0.05f, 0.0505f, 0.0501f, 1.0f };

    colors[ImGuiCol_PopupBg] = ImVec4{ 0.08f, 0.0805f, 0.081f, 1.0f };
}
