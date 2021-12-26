#ifndef IMGUI_L_H
#define IMGUI_L_H

class ImGuiLayer
{
public:
    static void Init(float width, float height);
    static void Destroy();

    static void Begin();
    static void End();
};

#endif
