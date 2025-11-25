#pragma once
#include "ImGuizmo.h"
#include "../Params/OperationParams.h"
#include "../ViewPortsController.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/HandleGizmoCommand.h"
#include "../Commands/MoveSelectedMeshesCommand.h"

class HandleGizmoCallBack : public Callback<GizmoParams>, public Observer
{
public:
    HandleGizmoCallBack(CommandRegistry* commandRegistry) : m_commandRegistry(commandRegistry) {}

    void scaleGizmo()
    {

    }

    void rotateGizmo()
    {

    }

    void moveGizmo(glm::mat4& gizmoTransform, glm::mat4& transform)
    {
        static bool makeMove = false;

        ImGuizmo::Manipulate(
            glm::value_ptr(ViewPortsHolderContext::s_camera->m_matrices.viewMatrix),
            glm::value_ptr(ViewPortsHolderContext::s_camera->m_matrices.perspectiveMatrix),
            ImGuizmo::OPERATION::TRANSLATE,
            ImGuizmo::LOCAL,
            glm::value_ptr(gizmoTransform),
            glm::value_ptr(transform));

        static glm::mat4 originalMatrix;
        if (ImGuizmo::IsUsing())
        {
            if (!makeMove)
            {
                originalMatrix = transform;
                makeMove = true;
            }
            return;
        }

        if (makeMove)
        {
            MoveSelectedMeshesParams meshParams;
            meshParams.moveByVector = glm::vec3(transform[3] - originalMatrix[3]);

            auto command = m_commandRegistry->getCommand<MoveSelectedMeshesCommand>();
            command->execute(meshParams);

            makeMove = false;
        }
    }

	void execute(const GizmoParams& iParams) override
	{
        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);

        int width, height, x, y;
        glfwGetWindowPos(Application::getWindow().getWindowHandle(), &x, &y);
        glfwGetFramebufferSize(Application::getWindow().getWindowHandle(), &width, &height);
        ImGuizmo::SetRect(x, y, (float)width, (float)height);

        SelectionController *selectionController = ViewPortsHolderContext::s_selectionController;
        const SelectionHolder &selectionHolder = selectionController->getHolder();
        const std::vector<Mesh *> &selectedMeshes = selectionHolder.meshes;

        if (selectedMeshes.empty())
        {
            return;
        }
        auto mesh = selectedMeshes.at(selectedMeshes.size() - 1);

        switch (iParams.m_type)
        {
            case ImGuizmo::OPERATION::TRANSLATE:
                moveGizmo(mesh->m_gizmoTransform, mesh->m_transform);
                break;
            case ImGuizmo::OPERATION::ROTATE:
                rotateGizmo();
                break;
            case ImGuizmo::OPERATION::SCALE:
            default:
                scaleGizmo();
                break;
        }
    }

private:
    CommandRegistry* m_commandRegistry = nullptr;
};
