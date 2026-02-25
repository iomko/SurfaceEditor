#include "ObjectsLayer.h"
#include "../ViewPortsController.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"
#include "LayerRegistry.h"

static AutoRegisterLayerArgs<ObjectsLayer, std::string> reg;

ObjectsLayer::ObjectsLayer(const std::string& name) : LayerWithID(name), m_windowSize{} {}

const ImGuiWindowFlags& ObjectsLayer::setWindowPosition()
{
    static constexpr const ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoCollapse;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    float width  = viewport->WorkSize.x * 0.1f;
    float height = viewport->WorkSize.y * 0.15f;

    m_windowSize = { width, height };

    float posX = viewport->WorkPos.x + (viewport->WorkSize.x - width) * 0.5f;
    float posY = viewport->WorkPos.y + (viewport->WorkSize.y - height) * 0.5f;

    ImGui::SetNextWindowSize(m_windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Always);

    return flags;
}

void ObjectsLayer::onImGuiRender()
{
    if (!VisibilityHandler::isVisible(OBJECTS))
    {
        return;
    }
    
    const ImGuiWindowFlags& flags = setWindowPosition();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 18.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.17f, 0.90f));

    ImGui::Begin(this->getName().c_str(), nullptr, flags);

    float buttonWidth  = m_windowSize.x * 0.75f;
    float buttonHeight = m_windowSize.y * 0.25f;
    float centerOffset = (m_windowSize.x - buttonWidth) * 0.5f;
    ImGui::SetCursorPosX(centerOffset);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 10));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.20f, 0.23f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.00f, 0.55f, 0.00f, 0.85f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.00f, 0.45f, 0.00f, 1.0f));

    const int styleColorApplied = 3;
    const int styleVarApplied = 2;

    if (ImGui::Button("Plane", ImVec2(buttonWidth, buttonHeight)))
    {
        //TODO
    }

    ImGui::Spacing();
    ImGui::SetCursorPosX(centerOffset);

    if (ImGui::Button("Cube", ImVec2(buttonWidth, buttonHeight)))
    {
        //TODO
    }

    ImGui::Spacing();
    ImGui::SetCursorPosX(centerOffset);

    if (ImGui::Button("Mesh", ImVec2(buttonWidth, buttonHeight)))
    {
        //TODO
    }

    ImGui::PopStyleColor(styleColorApplied);
    ImGui::PopStyleVar(styleVarApplied);

    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}