#include "DebugLayer.h"
#include "LayerRegistry.h"

#include "../Ml/Models/TriangleSkewMlModel.h"
#include "../ViewPortsController.h"
#include "../Core/Layer.h"
#include <vector>

static AutoRegisterLayer<DebugLayer, std::string> regDebugLayer;

DebugLayer::DebugLayer(const std::string &name)
    : Layer(name)
{
}

void DebugLayer::onEvent(Event &event)
{
    if (event.getType() == EventType::MouseButtonPress)
    {
        if (m_isMouseInsideWindow)
        {
            event.isHandled = true;
        }
    }
}

void DebugLayer::onImGuiRender()
{
    ImGui::Begin(this->getName().c_str());

    // Get window position and size
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 mousePos = ImGui::GetMousePos();

    // Update the class variable to track if the mouse is inside the window
    m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                             mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

    if (ImGui::Button("Recalculate"))
    {

        Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

        for (auto &[mesh, _] : scene->m_res.meshFaceOctreeCoordsMap)
        {

            for (ExtendedFace *face : mesh->m_halfEdgeStructure->m_faces)
            {

                face->m_isSkewed = false;

                // musime updatnut potom aj renderer data
                std::vector<FaceTriangleIndex> &triangleIndices = face->faceTriangleIndices;

                FaceTriangleIndex faceTriangleIndex = face->faceTriangleIndices.front();
                FaceTriangle &faceTriangle = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second.at(faceTriangleIndex);

                if (auto opt = mesh->bufferLayout.getTriangleBufferStorage(face->material))
                {

                    TriangleBufferStorage &triangleBufferStorage = opt->get();
                    std::vector<BufferStorageDataType::TriangleVertex> &triangleBufferVertices = triangleBufferStorage.data.vertices;

                    int faceIndexInVao = faceTriangle.indexInVAO;
                    for (int i = faceIndexInVao; i < faceIndexInVao + 3; ++i)
                    {
                        triangleBufferVertices.at(i).isSkewed = 0.0f;
                    }
                }
            }

            for (auto it = mesh->bufferLayout.triangleBuffersBegin(); it != mesh->bufferLayout.triangleBuffersEnd(); ++it)
            {
                TriangleBufferStorage &triangleBufferStorage = it->second;
                triangleBufferStorage.update();
            }
        }

        if (m_skewCheckboxState)
        {
            TriangleSkewModel triangleSkewModel;
            triangleSkewModel.loadModel();

            Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
            for (auto &[mesh, _] : scene->m_res.meshFaceOctreeCoordsMap)
            {
                triangleSkewModel.run(*mesh);
            }
        }
    }

    if (ImGui::Checkbox("TriangleSkew", &m_skewCheckboxState))
    {
    }

    ImGui::End();
}
