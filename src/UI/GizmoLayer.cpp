#include "GizmoLayer.h"
#include "../ViewPortsController.h"

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

void GizmoLayer::displayGizmo()
{
    ImGuizmo::BeginFrame();
    ImGuizmo::SetOrthographic(false);

    int width, height;
    glfwGetFramebufferSize(Application::getWindow().getWindowHandle(), &width, &height);
    ImGuizmo::SetRect(0, 0, (float)width, (float)height);

    SelectionController *selectionController = ViewPortsHolderContext::s_selectionController;
    const SelectionHolder &selectionHolder = selectionController->getHolder();
    const std::vector<Mesh *> &selectedMeshes = selectionHolder.meshes;

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
        glm::value_ptr(mesh->m_transform));
}