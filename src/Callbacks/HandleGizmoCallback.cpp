#include "HandleGizmoCallback.h"
#include "CallbackRegister.h"
#include "../ViewPortsController.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"

static AutoRegisterCallback<HandleGizmoCallBack> registerHandleGizmoCallback;

HandleGizmoCallBack::HandleGizmoCallBack(){}

glm::vec3 HandleGizmoCallBack::calcFaceMiddlePos(ExtendedFace *face)
{
    glm::vec3 sum(0.0f);
    int count = 0;

    for (auto it = face->faceVertexBegin(); it != face->faceVertexEnd(); ++it)
    {
        sum += (*it).m_position;
        ++count;
    }

    return sum / float(count);
}

void HandleGizmoCallBack::chooseSelectionMode(const GizmoParams &iParams, bool &end)
{
    SelectionController *selectionController = ViewPortsHolderContext::s_selectionController;
    const SelectionHolder &selectionHolder = selectionController->getHolder();
    const std::vector<Mesh *> &selectedMeshes = selectionHolder.meshes;

    if (iParams.m_selectionMode == GizmoParams::SelectionMode::Mesh)
    {
        bool modeSwitched = false;

        for (auto &mesh : selectedMeshes)
        {
            auto &faces = selectionHolder.faces.find(mesh)->second;

            if (!faces.empty())
            {
                modeSwitched = true;
                m_lastSelectedFacesCount = 0;

                for (auto &face : faces)
                {
                    ViewPortsHolderContext::s_selectionController->unregisterFace(mesh, face);
                }
            }
        }

        if (selectedMeshes.size() != m_lastSelectedMeshesCount || modeSwitched)
        {
            m_lastSelectedMeshesCount = selectedMeshes.size();
            if (!selectedMeshes.empty())
            {
                calcMiddlePos<Mesh *>(selectedMeshes, [](Mesh *mesh)
                                      { return glm::vec3(mesh->m_transform[3]); });
            }
        }
        if (selectedMeshes.empty())
        {
            end = true;
        }
    }
    else
    {
        std::vector<ExtendedFace *> selectedFaces;
        for (auto &selectedMesh : selectedMeshes)
        {
            const std::vector<ExtendedFace *> &faces = selectionHolder.faces.find(selectedMesh)->second;
            for (int i{}; i < faces.size(); ++i)
            {
                selectedFaces.push_back(faces[i]);
            }
        }
        if (selectedFaces.size() != m_lastSelectedFacesCount)
        {
            m_lastSelectedFacesCount = selectedFaces.size();
            if (!selectedFaces.empty())
            {
                calcMiddlePos<ExtendedFace *>(selectedFaces, [this](ExtendedFace *face)
                                              { return calcFaceMiddlePos(face); });
            }
        }
        if (selectedMeshes.empty() || selectedFaces.empty())
        {
            end = true;
        }
    }
}

void HandleGizmoCallBack::update() 
{
    if (m_selectionMode == GizmoParams::SelectionMode::Mesh)
    {
        MoveSelectedMeshesParams meshParams;
        meshParams.transformMatrix = m_realTimeTransform;

        auto* command = CommandRegistry::instance().getCommand(MOVE_SELECTED_MESHES_COMMAND);

        if(command)
            command->execute(meshParams);
    }
    else
    {
        MoveSelectedFacesParams faceParams;
        faceParams.moveByVector = m_realTimeTransform[3];

        auto* command = CommandRegistry::instance().getCommand(MOVE_SELECTED_FACES_COMMAND);
        if(command)
            command->execute(faceParams);
    }
}

void HandleGizmoCallBack::handleGizmo(ImGuizmo::OPERATION operation, std::function<void()> executeRealTime)
{
    static bool makeMove = false;

    ImGuizmo::Manipulate(
        glm::value_ptr(ViewPortsHolderContext::s_camera->m_matrices.viewMatrix),
        glm::value_ptr(ViewPortsHolderContext::s_camera->m_matrices.perspectiveMatrix),
        operation,
        ImGuizmo::LOCAL,
        glm::value_ptr(m_gizmoTransform),
        glm::value_ptr(m_transform));

    if (ImGuizmo::IsUsing())
    {
        if (!makeMove)
        {
            makeMove = true;
        }
        executeRealTime();

        return;
    }

    if (makeMove)
    {
        update();

        m_transform = glm::mat4(1.0f);
        m_realTimeTransform = glm::mat4(1.0f);
        makeMove = false;
    }
}

void HandleGizmoCallBack::execute(const GizmoParams &iParams)
{
    ImGuizmo::BeginFrame();
    ImGuizmo::SetOrthographic(false);

    int width, height, x, y;
    glfwGetWindowPos(Application::getWindow().getWindowHandle(), &x, &y);
    glfwGetFramebufferSize(Application::getWindow().getWindowHandle(), &width, &height);
    ImGuizmo::SetRect(x, y, (float)width, (float)height);

    bool end = false;
    chooseSelectionMode(iParams, end);

    if (end)
    {
        return;
    }

    m_selectionMode = iParams.m_selectionMode;

    switch (iParams.m_type)
    {
    case ImGuizmo::OPERATION::TRANSLATE:
        handleGizmo(
            iParams.m_type,
            m_selectionMode == GizmoParams::SelectionMode::Mesh ? std::function<void()>([this]
                                                                                        { m_realTimeTransform[3] += glm::vec4(glm::vec3(m_transform[3]), 0.0f); })
                                                                : std::function<void()>([this]
                                                                                        {
                        m_realTimeTransform = glm::mat4(1.0f);
                        m_realTimeTransform[3] += glm::vec4(glm::vec3(m_transform[3]), 0.0f);
                        update(); }));
        break;
    case ImGuizmo::OPERATION::ROTATE:
        handleGizmo(
            iParams.m_type,
            [this]()
            {
                m_realTimeTransform *= m_transform;
            });
        break;
    case ImGuizmo::OPERATION::SCALE:
    default:
        handleGizmo(
            iParams.m_type,
            [this]()
            {
                glm::vec3 pivot = glm::vec3(m_gizmoTransform[3]);
                glm::mat4 pivotMat = glm::translate(glm::mat4(1.0f), pivot);
                glm::mat4 negPivotMat = glm::translate(glm::mat4(1.0f), -pivot);

                m_realTimeTransform = pivotMat * m_transform * negPivotMat * m_realTimeTransform;
            });
        break;
    }
}
