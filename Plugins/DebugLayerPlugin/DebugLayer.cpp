#include "DebugLayer.h"
#include "../../src/UI/LayerRegistry.h"
#include "../../src/Ml/Analyser/AnalyserRegistry.h"

#include "../../src/ViewPortsController.h"
#include "../../src/Core/Layer.h"
#include "../../src/Renderer/MaterialRegistry.h"
#include <vector>
#include <cassert>

#include "../../src/ViewPortsController.h"

#include "../../src/Ml/Analyser/AnalyserExecutionLayerRegistry.h"

static AutoRegisterLayerArgs<DebugLayer, std::string> regDebugLayer("DEBUG_LAYER");

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

    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 mousePos = ImGui::GetMousePos();

    m_isMouseInsideWindow =
        (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
         mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

    m_analyserInstances = AnalyserRegistry::instance().getAnalyserInstances();

    if (m_selectedAnalyserName.first.empty() && !m_analyserInstances.empty())
    {
        m_selectedAnalyserName.first = m_analyserInstances.front().displayName;
        m_selectedAnalyserName.second = m_analyserInstances.front().instanceId;
    }

    ImGui::Text("Neural network model:");
    if (ImGui::BeginCombo("##ModelDropdown", m_selectedAnalyserName.first.empty() ? "None" : m_selectedAnalyserName.first.c_str()))
    {
        for (const auto& instanceInfo : m_analyserInstances)
        {
            bool isSelected = (m_selectedAnalyserName.second == instanceInfo.instanceId);

            if (ImGui::Selectable(instanceInfo.displayName.c_str(), isSelected))
            {
                m_selectedAnalyserName.first = instanceInfo.displayName;
                m_selectedAnalyserName.second = instanceInfo.instanceId;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    auto* selectedAnalyser = AnalyserRegistry::instance().getAnalyserInstance(m_selectedAnalyserName.second);


    if (selectedAnalyser) {
        std::string selectedTypeId =
            AnalyserRegistry::instance().getInstanceTypeId(m_selectedAnalyserName.second);

        auto* executionLayer =
            AnalyserExecutionLayerRegistry::instance().getExecutionLayer(selectedTypeId);

        if (executionLayer) {
            executionLayer->onImGuiRender(selectedAnalyser);
        } else {
            ImGui::Separator();

            if (auto* predictor = dynamic_cast<Predictor*>(selectedAnalyser)) {
                if (ImGui::Button("Predict")) {
                    const SceneResources::MeshFaceOctreeCoordsMap& meshes =
                        ViewPortsHolderContext::s_viewPortsController
                            ->m_scene
                            ->m_res
                            .meshFaceOctreeCoordsMap;

                    for (auto& [mesh, _] : meshes) {
                        predictor->predict(mesh);
                    }
                }
            } else if (auto* processor = dynamic_cast<Processor*>(selectedAnalyser)) {
                if (ImGui::Button("Process")) {
                    const SceneResources::MeshFaceOctreeCoordsMap& meshes =
                        ViewPortsHolderContext::s_viewPortsController
                            ->m_scene
                            ->m_res
                            .meshFaceOctreeCoordsMap;

                    for (auto& [mesh, _] : meshes) {
                        processor->process(mesh);
                    }
                }
            }
        }
    }

    ImGui::End();
}

void DebugLayer::updateEdgesVaoData(Mesh *mesh) {
    Material* defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");
    if(auto opt = mesh->bufferLayout.getLineBufferStorage(defaultLineMaterial)) {
        LineBufferStorage& lineBufferStorage = opt->get();
        lineBufferStorage.update();
    }
}

void DebugLayer::updateFacesVaoData(Mesh *mesh) {
    Material* defaultMeshMaterial = MaterialRegistry::getMaterial("defaultMeshMaterial");
    if(auto opt = mesh->bufferLayout.getTriangleBufferStorage(defaultMeshMaterial)) {
        TriangleBufferStorage& triangleBufferStorage = opt->get();
        triangleBufferStorage.update();
    }
}

void DebugLayer::unhighlightAllFaces(Mesh *mesh) {
    for (ExtendedFace* face: mesh->m_halfEdgeStructure->m_faces) {
        face->m_isSkewed = false;

        FaceTriangleIndex faceTriangleIndex = face->faceTriangleIndices.front();
        FaceTriangle& faceTriangle = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second.at(faceTriangleIndex);

        if(auto opt = mesh->bufferLayout.getTriangleBufferStorage(face->material)) {

            TriangleBufferStorage& triangleBufferStorage = opt->get();
            std::vector<BufferStorageDataType::TriangleVertex>& triangleBufferVertices = triangleBufferStorage.data.vertices;

            int faceIndexInVao = faceTriangle.indexInVAO;
            for (int i = faceIndexInVao; i < faceIndexInVao + 3; ++i)
            {
                triangleBufferVertices.at(i).isSkewed = 0.0f;
            }
        }
    }
}