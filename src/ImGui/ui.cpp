#include "ui.h"

#include "events/eventDispatcher.h"
#include "graphics/window.h"

#include <imgui.h>

#define IMGImGuiLayer_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

static float s_width = 0, s_height = 0;

void ImGuiLayer::Init(float width, float height)
{
    LOG_TRACE("Initializing ImGui");

    s_width = width;
    s_height = height;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    // //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    float fontSize = 22.0f;// *2.0f;
	io.Fonts->AddFontFromFileTTF("res/fonts/firacode/FiraCode-Bold.ttf", fontSize);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/firacode/FiraCode-Regular.ttf", fontSize);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;



	// Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetNativeWindow());
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    EventHandler<WindowResize>::RegisterOnEventFunction(ImGuiLayer::OnEvent<WindowResize>);
    EventHandler<KeyPressed>::RegisterOnEventFunction(ImGuiLayer::OnEvent<KeyPressed>);
    EventHandler<KeyReleased>::RegisterOnEventFunction(ImGuiLayer::OnEvent<KeyReleased>);
    EventHandler<MouseButtonPressed>::RegisterOnEventFunction(ImGuiLayer::OnEvent<MouseButtonPressed>);
    EventHandler<MouseButtonReleased>::RegisterOnEventFunction(ImGuiLayer::OnEvent<MouseButtonReleased>);
    EventHandler<MouseMoved>::RegisterOnEventFunction(ImGuiLayer::OnEvent<MouseMoved>);
    EventHandler<MouseScrolled>::RegisterOnEventFunction(ImGuiLayer::OnEvent<MouseScrolled>);
}

void ImGuiLayer::Destroy()
{

}

void ImGuiLayer::OnUpdate(float ts)
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

template<>
bool ImGuiLayer::OnEvent<WindowResize>(WindowResize event)
{
    s_width = event.width;
    s_height = event.height;
    return false;
}

template<>
bool ImGuiLayer::OnEvent<KeyPressed>(KeyPressed event)
{
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureKeyboard;
}

template<>
bool ImGuiLayer::OnEvent<KeyReleased>(KeyReleased event)
{
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureKeyboard;
}

template<>
bool ImGuiLayer::OnEvent<MouseButtonPressed>(MouseButtonPressed event)
{
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

template<>
bool ImGuiLayer::OnEvent<MouseButtonReleased>(MouseButtonReleased event)
{
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

template<>
bool ImGuiLayer::OnEvent<MouseScrolled>(MouseScrolled event)
{
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

template<>
bool ImGuiLayer::OnEvent<MouseMoved>(MouseMoved event)
{
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}
