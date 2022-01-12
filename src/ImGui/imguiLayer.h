#ifndef IMGUI_L_H
#define IMGUI_L_H

#include "events/eventSystem.h"

class ImGuiLayer
{
public:
    static void Init(float width, float height, EventSystem* eventSystem);
    static void Destroy();

    static void Begin();
    static void End();
};

#endif
