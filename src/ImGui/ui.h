#ifndef UI_H
#define UI_H

#include "imgui.h"
#include <string>

// These are a few shorthands for windows in a style I like

namespace UI
{
    void BeginDockSpaceWindow(const std::string& name, ImVec2 minSize, ImVec2 maxSize, ImVec2 position, ImGuiDockNodeFlags dockFlags = 0, ImGuiWindowFlags windowFlags = 0);
    void EndDockSpaceWindow();

    void BeginWindow(const std::string& name, bool* open = nullptr, ImGuiWindowFlags windowFlags = 0);
    void EndWindow();

    bool TreeNode(void* id, const std::string& name, ImGuiTreeNodeFlags flags = 0);

    extern ImFont* FontDefault;
    extern ImFont* FontBold;
};

#endif
