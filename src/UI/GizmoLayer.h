#pragma once
#include "ImGuizmo.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"
#include "../ViewPortsController.h"

class GizmoLayer : public Layer, public Observable
{
private:
    GizmoLayerParams::Type m_type = GizmoLayerParams::Type::Disable;
    GizmoParams::SelectionMode m_selectionMode = GizmoParams::SelectionMode::Mesh;
    CommandRegistry* m_commandRegistry;

public:
    GizmoLayer(const std::string& name, CommandRegistry* commandRegistry) : Layer(name), m_commandRegistry(commandRegistry) {}

    void onImGuiRender() override
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

            auto command = m_commandRegistry->getCommand<HandleGizmoCommand>();

            command->execute(gizmoParams);
        }

        ImGui::End();
    }

    ImGuizmo::OPERATION operation()
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
};
