#pragma once
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"
#include "Callback.h"
#include "../Patterns/Observer.h"
#include "../ViewPortsController.h"

class GizmoLayerCallback : public Callback<GizmoLayerParams>, public Observer
{
public:
    virtual void execute(const GizmoLayerParams& params)
    {
        std::cout << "Executing callback\n";

        GizmoLayerParams::Type gizmoType = params.m_type;
        ImGuizmo::OPERATION operation;

         if (gizmoType == GizmoLayerParams::Disable)
         {
            return;
         }
         else if (gizmoType == GizmoLayerParams::Move)
         {
            operation = ImGuizmo::TRANSLATE;
         }
         else if (gizmoType == GizmoLayerParams::Rotate)
         {
            operation = ImGuizmo::ROTATE;
         }
         else if (gizmoType == GizmoLayerParams::Scale)
         {
            operation = ImGuizmo::SCALE;
         }

        displayGizmo(operation);
    }

    void displayGizmo(ImGuizmo::OPERATION& operation)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);

        glm::mat4 model = glm::mat4(1.0f);
        int width, height;
        glfwGetFramebufferSize(Application::getWindow().getWindowHandle(), &width, &height);
        ImGuizmo::SetRect(0, 0, (float)width, (float)height);

        ImGuizmo::Manipulate(
            glm::value_ptr(ViewPortsHolderContext::s_camera->m_matrices.viewMatrix),
            glm::value_ptr(ViewPortsHolderContext::s_camera->m_matrices.perspectiveMatrix),
            operation,
            ImGuizmo::LOCAL,
            glm::value_ptr(model)
        );

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};