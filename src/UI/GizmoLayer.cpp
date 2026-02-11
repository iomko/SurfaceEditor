#include "GizmoLayer.h"
#include "../ViewPortsController.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"

static AutoRegisterLayerArgs<GizmoLayer, std::string> reg;

GizmoLayer::GizmoLayer(const std::string &name) : LayerWithID(name) {}

void GizmoLayer::onImGuiRender()
{
    ImGui::Begin(this->getName().c_str());

    int typeModeInt = static_cast<int>(m_type);
    if (ImGui::RadioButton("Move", &typeModeInt, GizmoLayerParams::Type::Move) ||
        ImGui::RadioButton("Rotate", &typeModeInt, GizmoLayerParams::Type::Rotate) ||
        ImGui::RadioButton("Scale", &typeModeInt, GizmoLayerParams::Type::Scale) ||
        ImGui::RadioButton("Disable", &typeModeInt, GizmoLayerParams::Type::Disable))
    {
        m_type = static_cast<GizmoLayerParams::Type>(typeModeInt);
    }

    int selectionMode = static_cast<int>(m_selectionMode);
    ImGui::Text("Selection mode:");
    if (ImGui::RadioButton("Mesh", &selectionMode, GizmoParams::SelectionMode::Mesh) ||
        ImGui::RadioButton("Face", &selectionMode, GizmoParams::SelectionMode::Face))
    {
        m_selectionMode = static_cast<GizmoParams::SelectionMode>(selectionMode);
    }

    if (m_type != GizmoLayerParams::Type::Disable)
    {
        GizmoParams gizmoParams;
        gizmoParams.m_type = operation();
        gizmoParams.m_selectionMode = m_selectionMode;

        auto *command = CallableRegistry::getCommand(HANDLE_GIZMO_COMMAND);
        if (command)
            command->execute(gizmoParams);
    }

    ImGui::End();
}

ImGuizmo::OPERATION GizmoLayer::operation()
{
    switch (m_type)
    {
    case GizmoLayerParams::Move:
        return ImGuizmo::TRANSLATE;
    case GizmoLayerParams::Rotate:
        return ImGuizmo::ROTATE;
    case GizmoLayerParams::Scale:
    default:
        return ImGuizmo::SCALE;
    }
}
