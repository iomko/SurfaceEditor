#include "ObjectsLayer.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"
#include "LayerRegistry.h"

static AutoRegisterLayerArgs<ObjectsLayer, std::string> reg;

ObjectsLayer::ObjectsLayer(const std::string& name)
    : LayerWithID(name), m_windowSize{}
{
    auto& layerRegistry = LayerRegistry::instance();
    auto layer = layerRegistry.getLayer(OBJECT_MANIPULATION_LAYER, "ObjectManipulationLayer");
    auto objectManipulationLayer = static_cast<ObjectManipulationLayer*>(layer);

    m_windowPos = objectManipulationLayer->rightBottomCorner();
}

void ObjectsLayer::setWindowSizeAndPosition()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    float width  = viewport->WorkSize.x * 0.05f;
    float height = viewport->WorkSize.y * 0.075f;

    m_windowSize = { width, height };

    ImGui::SetNextWindowSize(m_windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(*m_windowPos, ImGuiCond_Always);
}

void ObjectsLayer::onImGuiRender()
{
    if (!VisibilityHandler::isVisible(OBJECTS))
    {
        return;
    }
    
    setWindowSizeAndPosition();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    static const ImGuiWindowFlags flags = WindowStyle::defaultWindow();
    WindowStyle::setup(this->getName().c_str(), flags);


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


    WindowStyle::end();
}