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

            GizmoLayerParams gizmoParams;
            gizmoParams.m_type = m_type;
            notifyObservers(gizmoParams);
        }

        ImGui::End();

        if (m_type != GizmoLayerParams::Type::Disable)
        {
            displayGizmo();
        }
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

    void displayGizmo()
    {
        static bool makeMove = false;

        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);

        int width, height, x, y;
        glfwGetWindowPos(Application::getWindow().getWindowHandle(), &x, &y);
        glfwGetFramebufferSize(Application::getWindow().getWindowHandle(), &width, &height);
        ImGuizmo::SetRect(x, y, (float)width, (float)height);

        SelectionController*  selectionController = ViewPortsHolderContext::s_selectionController;
		const SelectionHolder& selectionHolder = selectionController->getHolder();
		const std::vector<Mesh*>& selectedMeshes = selectionHolder.meshes;

        if (selectedMeshes.empty())
        {
            return;
        }
        auto mesh = selectedMeshes.at(selectedMeshes.size() - 1);

        ImGuizmo::Manipulate(
            glm::value_ptr(ViewPortsHolderContext::s_camera->m_matrices.viewMatrix),
            glm::value_ptr(ViewPortsHolderContext::s_camera->m_matrices.perspectiveMatrix),
            operation(),
            ImGuizmo::LOCAL,
            glm::value_ptr(mesh->m_gizmoTransform)
        );

        static glm::mat4 originalMatrix;
        if (ImGuizmo::IsUsing())
        {
            if (!makeMove)
            {
                originalMatrix = mesh->m_gizmoTransform;
                makeMove = true;
            }
            return;
        }

        if (makeMove)
        {
            VertexParams vertexParams;
            vertexParams.mesh = mesh;
            vertexParams.newPosition = glm::vec3(mesh->m_gizmoTransform[3] - originalMatrix[3]);

            auto command = m_commandRegistry->getCommand<MoveVertexCommand>();

            auto& vertices = mesh->m_halfEdgeStructure->m_vertices;
            
            for (auto& vertex : vertices)
            {
                vertexParams.vertex = vertex;
                command->execute(vertexParams);
            }
            
            mesh->m_gizmoTransform = glm::mat4(1.0f);
            makeMove = false;
        }
    }
};
