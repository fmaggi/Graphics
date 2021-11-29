#include "events/event.h"

#include "log/log.h"

class ImGuiLayer
{
public:
    static void Init(float width, float height);
    static void Destroy();

    static void OnUpdate(float ts);

    static void Begin();
    static void End();

    template<typename T>
    static bool OnEvent(T event)
    {
        ASSERT(false, "Invalid event type passed to ImGuiLayer");
    }
};

template<>
bool ImGuiLayer::OnEvent<WindowResize>(WindowResize event);

template<>
bool ImGuiLayer::OnEvent<KeyPressed>(KeyPressed event);

template<>
bool ImGuiLayer::OnEvent<KeyReleased>(KeyReleased event);

template<>
bool ImGuiLayer::OnEvent<MouseButtonPressed>(MouseButtonPressed event);

template<>
bool ImGuiLayer::OnEvent<MouseButtonReleased>(MouseButtonReleased event);

template<>
bool ImGuiLayer::OnEvent<MouseScrolled>(MouseScrolled event);

template<>
bool ImGuiLayer::OnEvent<MouseMoved>(MouseMoved event);
