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

    void calcMiddlePos(const std::vector<Mesh*>& selectedMeshes)
    {
        glm::mat4 middle(1.0f);

        for (const auto& mesh : selectedMeshes)
        {
            mesh->m_realTimeTransform = &m_realTimeTransform;

            middle[3] += glm::vec4(glm::vec3(mesh->m_transform[3]), 0.0f);
        }

        middle[3] /= selectedMeshes.size();
        middle[3][3] = 1.0f;

        m_gizmoTransform[3] = middle[3];

        for (int i{}; i < 4; ++i)
        {
            std::cout << m_gizmoTransform[3][i] << "\n";
        }
        std::cout << "\n";
    }

    void scaleGizmo()
    {

    }

    void rotateGizmo()
    {

    }

    void moveGizmo()
    {
        static bool makeMove = false;

        ImGuizmo::Manipulate(
            glm::value_ptr(ViewPortsHolderContext::s_camera->m_matrices.viewMatrix),
            glm::value_ptr(ViewPortsHolderContext::s_camera->m_matrices.perspectiveMatrix),
            ImGuizmo::OPERATION::TRANSLATE,
            ImGuizmo::LOCAL,
            glm::value_ptr(m_gizmoTransform),
            glm::value_ptr(m_transform));

        if (ImGuizmo::IsUsing())
        {
            if (!makeMove)
            {
                makeMove = true;
            }
            m_realTimeTransform[3].x += m_transform[3].x;
            m_realTimeTransform[3].y += m_transform[3].y;
            m_realTimeTransform[3].z += m_transform[3].z;

            return;
        }

        if (makeMove)
        {
            MoveSelectedMeshesParams meshParams;
            meshParams.moveByVector = m_realTimeTransform[3];

            auto command = m_commandRegistry->getCommand<MoveSelectedMeshesCommand>();
            command->execute(meshParams);

            m_transform = glm::mat4(1.0f);
            m_realTimeTransform = glm::mat4(1.0f);
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

        SelectionController* selectionController = ViewPortsHolderContext::s_selectionController;
        const SelectionHolder& selectionHolder = selectionController->getHolder();
        const std::vector<Mesh*>& selectedMeshes = selectionHolder.meshes;

        if (selectedMeshes.empty())
        {
            return;
        }
        if (selectedMeshes.size() != m_lastSelectedMeshesCount)
        {
            std::cout << "SELECTED MESHES SIZE: " << selectedMeshes.size() << "\n";
            std::cout << "LAST MESHES SIZE: " << m_lastSelectedMeshesCount << "\n";

            m_lastSelectedMeshesCount = selectedMeshes.size();
            calcMiddlePos(selectedMeshes);
        }

        switch (iParams.m_type)
        {
            case ImGuizmo::OPERATION::TRANSLATE:
                moveGizmo();
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
    glm::mat4 m_realTimeTransform = glm::mat4(1.0f);
    glm::mat4 m_gizmoTransform = glm::mat4(1.0f);
    glm::mat4 m_transform = glm::mat4(1.0f);
    int m_lastSelectedMeshesCount = 0;
};
