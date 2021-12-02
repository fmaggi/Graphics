#include "ui.h"

#include "imgui_internal.h"
#include "log/log.h"

namespace UI
{

    ImFont* FontDefault;
    ImFont* FontBold;

    void BeginDockSpaceWindow(const std::string& name, ImVec2 minSize, ImVec2 maxSize, ImVec2 position, ImGuiDockNodeFlags dockFlags, ImGuiWindowFlags windowFlags)
    {
        static const ImGuiDockNodeFlags defualtDockFlags = ImGuiDockNodeFlags_None;
        static const ImGuiWindowFlags defaultWindowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse;

        windowFlags |= defaultWindowFlags;
        dockFlags |= defualtDockFlags;

        ImGui::SetNextWindowPos(position);
        ImGui::SetNextWindowSizeConstraints(minSize, maxSize);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin(name.c_str(), nullptr, windowFlags);
        ImGui::PopStyleVar();

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID((name + "_dock").c_str());
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockFlags);
        }

        style.WindowMinSize.x = minWinSizeX;
    }

    void EndDockSpaceWindow()
    {
        ImGui::End();
    }

    void BeginWindow(const std::string& name, bool* open,  ImGuiWindowFlags windowFlags)
    {
        ImGui::Begin(name.c_str(), open, windowFlags);
    }

    void EndWindow()
    {
        ImGui::End();
    }

    bool TreeNode(void* id, const std::string& name, ImGuiTreeNodeFlags flags)
    {
        static const ImGuiTreeNodeFlags defaultTreeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth;
        flags |= flags;

        return ImGui::TreeNodeEx(id, flags, name.c_str());
    }

};
