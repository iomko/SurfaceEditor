#include "HandleGizmoCallback.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/ViewPortsController.h"
#include "../../src/Commands/CommandRegistry.h"

static AutoRegisterCallback<HandleGizmoCallBack> registerHandleGizmoCallback("HANDLE_GIZMO_CALLBACK");

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

void HandleGizmoCallBack::init(bool& earlyReturn)
{
    earlyReturn = false;
    auto selectionController    = ViewPortsHolderContext::s_selectionController.get();
    const auto& selectionHolder = selectionController->getHolder();

    if (m_selectionMode == SelectionMode::MESH)
    {
        earlyReturn = selectionHolder.meshes.empty();
        if (selectionController->newSelectionEvent() && !earlyReturn)
        {
            calcMiddlePos<Mesh*>(selectionHolder.meshes, [](Mesh* mesh) {
                return glm::vec3(mesh->m_transform[3]);
            });
        }
    }
    else if (m_selectionMode == SelectionMode::FACE)
    {
        earlyReturn = selectionHolder.faces.empty();
        if (selectionController->newSelectionEvent() && !earlyReturn)
        {
            std::vector<ExtendedFace*> selectedFaces;
            for (auto &mesh : selectionHolder.meshes)
            {
                const std::vector<ExtendedFace*>& faces = selectionHolder.faces.find(mesh)->second;
                for (int i{}; i < faces.size(); ++i)
                {
                    selectedFaces.push_back(faces[i]);
                }
            }

            calcMiddlePos<ExtendedFace*>(selectedFaces, [this](ExtendedFace* face) {
                return calcFaceMiddlePos(face);
            });
        }
    }

    selectionController->setNewSelectionEvent(false);
}

void HandleGizmoCallBack::update() 
{
    if (m_selectionMode == SelectionMode::MESH)
    {
        MoveSelectedMeshesParams meshParams;
        meshParams.transformMatrix = m_realTimeTransform;

        auto* command = CommandRegistry::instance().getCommand("MOVE_SELECTED_MESHES_COMMAND");

        if (command)
        {
            command->execute(meshParams);
        }
    }
    else
    {
        MoveSelectedFacesParams faceParams;
        faceParams.moveByVector = m_realTimeTransform[3];

        auto* command = CommandRegistry::instance().getCommand("MOVE_SELECTED_FACES_COMMAND");
        if(command)
        {
            command->execute(faceParams);
        }
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

    bool earlyReturn{};
    init(earlyReturn);

    if (earlyReturn)
    {
        return;
    }

    m_selectionMode = ViewPortsHolderContext::s_selectionController->selectionMode();
    std::function<void()> realTimeCallback;

    switch (iParams.m_type)
    {
    case ImGuizmo::OPERATION::TRANSLATE:
        if (m_selectionMode == SelectionMode::MESH)
        {
            realTimeCallback = [this]() {
                m_realTimeTransform[3] += glm::vec4(glm::vec3(m_transform[3]), 0.0f);
            };
        }
        else
        {
            realTimeCallback = [this]() {
                m_realTimeTransform = glm::mat4(1.0f);
                m_realTimeTransform[3] += glm::vec4(glm::vec3(m_transform[3]), 0.0f);
                update();
            };
        }
        break;
    case ImGuizmo::OPERATION::ROTATE:
        realTimeCallback = [this]() {
            m_realTimeTransform *= m_transform;
        };
        break;
    case ImGuizmo::OPERATION::SCALE:
        realTimeCallback = [this]() {
            glm::vec3 pivot = glm::vec3(m_gizmoTransform[3]);
            glm::mat4 pivotMat = glm::translate(glm::mat4(1.0f), pivot);
            glm::mat4 negPivotMat = glm::translate(glm::mat4(1.0f), -pivot);

            m_realTimeTransform = pivotMat * m_transform * negPivotMat * m_realTimeTransform;
        };
        break;
    default:
        break;
    }

    handleGizmo(iParams.m_type, realTimeCallback);
}
