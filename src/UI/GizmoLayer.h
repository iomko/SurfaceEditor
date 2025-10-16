#pragma once
#include "ImGuizmo.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"

class GizmoLayer : public Layer, public Observable
{
private:
    GizmoLayerParams::Type m_type = GizmoLayerParams::Type::Disable;
    Camera* m_camera;
    glm::mat4 m_model = glm::mat4(1.0f);

public:
    GizmoLayer(const std::string& name, Camera* camera) : Layer(name), m_camera(camera) {}

    void onImGuiRender() override
    {
        ImGui::Begin(this->getName().c_str());

        int typeModeInt = static_cast<int>(m_type);
        if (ImGui::RadioButton("Move", &typeModeInt, GizmoLayerParams::Type::Move) ||
            ImGui::RadioButton("Rotate", &typeModeInt, GizmoLayerParams::Type::Rotate) ||
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

    void displayGizmo()
    {
        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);

        int width, height;
        glfwGetFramebufferSize(Application::getWindow().getWindowHandle(), &width, &height);
        ImGuizmo::SetRect(0, 0, (float)width, (float)height);

        
        ImGuizmo::Manipulate(
            glm::value_ptr(m_camera->m_matrices.viewMatrix),
            glm::value_ptr(m_camera->m_matrices.perspectiveMatrix),
            ImGuizmo::TRANSLATE,
            ImGuizmo::LOCAL,
            glm::value_ptr(m_model)
        );
    }
};
