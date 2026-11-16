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

#include <glm/gtc/matrix_transform.hpp>

#include "../../src/Commands/CommandRegistry.h"
#include "../../src/Core/Profiler.h"
#include <algorithm>

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

    if (ImGui::TreeNode("ML"))
    {
        ImGui::Text("Neural network model:");
        if (ImGui::BeginCombo("##ModelDropdown", m_selectedAnalyserName.first.empty() ? "None" : m_selectedAnalyserName.first.c_str()))
        {
            for (const auto &instanceInfo : m_analyserInstances)
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

        auto *selectedAnalyser = AnalyserRegistry::instance().getAnalyserInstance(m_selectedAnalyserName.second);

        if (selectedAnalyser)
        {
            std::string selectedTypeId =
                AnalyserRegistry::instance().getInstanceTypeId(m_selectedAnalyserName.second);

            auto *executionLayer =
                AnalyserExecutionLayerRegistry::instance().getExecutionLayer(selectedTypeId);

            if (executionLayer)
            {
                executionLayer->onImGuiRender(selectedAnalyser);
            }
            else
            {
                ImGui::Separator();

                if (auto *predictor = dynamic_cast<Predictor *>(selectedAnalyser))
                {
                    if (ImGui::Button("Predict"))
                    {
                        const auto &meshes =
                            ViewPortsHolderContext::s_viewPortsController
                                ->m_scene->getAllMeshes();

                        for (auto &mesh : meshes)
                        {
                            predictor->predict(mesh);
                        }
                    }
                }
                else if (auto *processor = dynamic_cast<Processor *>(selectedAnalyser))
                {
                    if (ImGui::Button("Process"))
                    {
                        const auto &meshes =
                            ViewPortsHolderContext::s_viewPortsController
                                ->m_scene->getAllMeshes();

                        for (auto &mesh : meshes)
                        {
                            processor->process(mesh);
                        }
                    }
                }
            }
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Profiler"))
    {
        // Num of runs
        static int numOfRuns = 15;
        ImGui::Text("Number of runs");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        ImGui::InputInt("##numOfRuns", &numOfRuns);

        auto addPlaneCommand = CommandRegistry::instance().getCommand("ADD_PLANE_COMMAND");
        if (addPlaneCommand != nullptr)
        {
            // AddPlane
            static bool addPlane = false;
            ImGui::Text("Add Plane");
            ImGui::SameLine();
            ImGui::Checkbox("##addPlane", &addPlane);

            // Move Plane
            static bool movePlane = false;
            ImGui::Text("Move Plane");
            ImGui::SameLine();
            ImGui::Checkbox("##movePlane", &movePlane);
            // Rotate Plane
            static bool rotatePlane = false;
            ImGui::Text("Rotate Plane");
            ImGui::SameLine();
            ImGui::Checkbox("##rotatePlane", &rotatePlane);
            // Scale Plane
            static bool scalePlane = false;
            ImGui::Text("Scale Plane");
            ImGui::SameLine();
            ImGui::Checkbox("##scalePlane", &scalePlane);
            // Solidify plane
            static bool solidifyPlane = false;
            ImGui::Text("Solidify Plane");
            ImGui::SameLine();
            ImGui::Checkbox("##solidifyPlane", &solidifyPlane);
            // Create Print
            static bool createPrint = false;
            ImGui::Text("Create Print");
            ImGui::SameLine();
            ImGui::Checkbox("##createPrint", &createPrint);
            // Delete Plane
            static bool deletePlane = false;
            ImGui::Text("Delete Plane");
            ImGui::SameLine();
            ImGui::Checkbox("##deletePlane", &deletePlane);
            // Export Plane
            static bool exportPlane = false;
            ImGui::Text("Export Plane");
            ImGui::SameLine();
            ImGui::Checkbox("##exportPlane", &exportPlane);
            // Import Plane
            static bool importPlane = false;
            ImGui::Text("Import Plane");
            ImGui::SameLine();
            ImGui::Checkbox("##importPlane", &importPlane);

            if (ImGui::Button("Run profiling tests"))
            {
                for (int i = 0; i < numOfRuns; ++i)
                {
                    Mesh *planeMesh = nullptr;
                    SE_PROFILE_SCOPE_N("DebugLayer-Profiler");
                    if (movePlane || rotatePlane || scalePlane || solidifyPlane || createPrint || deletePlane || exportPlane)
                        addPlane = true;
                    if (addPlane)
                    {
                        PlaneParams addPlaneParams;
                        addPlaneParams.m_position = {0, 0, 0};
                        addPlaneParams.m_size = 50;
                        addPlaneParams.m_subdivisionLevel = 50;
                        Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
                        std::vector<Mesh *> meshesBefore;
                        for (const auto &mesh : scene->getAllMeshes())
                        {
                            meshesBefore.push_back(mesh);
                        }
                        addPlaneCommand->execute(addPlaneParams);

                        // SELECT MESH
                        for (const auto &mesh : scene->getAllMeshes())
                        {
                            if (std::find(meshesBefore.begin(), meshesBefore.end(), mesh) == meshesBefore.end())
                            {
                                auto *selectionController = ViewPortsHolderContext::s_selectionController.get();
                                selectionController->registerMesh(mesh);
                                planeMesh = mesh;
                                selectionController->setNewSelectionEvent(true);
                                break;
                            }
                        }
                    }
                    auto *moveMeshesCommand = CommandRegistry::instance().getCommand("MOVE_SELECTED_MESHES_COMMAND");
                    if (movePlane)
                    {
                        if (moveMeshesCommand)
                        {

                            MoveSelectedMeshesParams moveMeshParams;
                            glm::mat4 transformMatrix = glm::mat4(1.0f);
                            glm::mat4 m_transform(
                                1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f,
                                1.0f, 0.0f, 0.0f, 1.0f);
                            transformMatrix[3] += glm::vec4(glm::vec3(m_transform[3]), 0.0f);
                            moveMeshParams.transformMatrix = transformMatrix;

                            moveMeshesCommand->execute(moveMeshParams);
                        }
                    }
                    if (rotatePlane)
                    {
                        if (moveMeshesCommand)
                        {
                            MoveSelectedMeshesParams rotateParams;
                            rotateParams.transformMatrix = glm::rotate(
                                glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                            moveMeshesCommand->execute(rotateParams);
                        }
                    }
                    if (scalePlane)
                    {
                        if (moveMeshesCommand)
                        {
                            MoveSelectedMeshesParams scaleParams;
                            scaleParams.transformMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
                            moveMeshesCommand->execute(scaleParams);
                        }
                    }
                    if (solidifyPlane)
                    {
                        auto *solidifyMeshesCommand = CommandRegistry::instance().getCommand("SOLIDIFY_MESHES_COMMAND");
                        if (solidifyMeshesCommand)
                        {
                            solidifyMeshesCommand->execute();
                        }
                    }
                    if (createPrint)
                    {
                        auto *createPrintCommand = CommandRegistry::instance().getCommand("CREATE_PRINT_COMMAND");
                        if (createPrintCommand)
                        {
                            PrintMeshSettingsParams printParams;
                            printParams.mesh = planeMesh;

                            createPrintCommand->execute(printParams);
                        }
                    }
                    if (exportPlane)
                    {
                        auto *exportMeshesCommand = CommandRegistry::instance().getCommand("EXPORT_MESHES_COMMAND");
                        if (exportMeshesCommand)
                        {
                            ImportExportMeshesParams exportParams;
                            exportParams.m_filePathMeshes = "profiler_import_export_" + std::to_string(i);
                            exportParams.exportOnlySelected = true;
                            exportMeshesCommand->execute(exportParams);
                        }
                    }
                    if (deletePlane)
                    {
                        auto *deleteSelectedMeshesCommand = CommandRegistry::instance().getCommand("DELETE_SELECTED_MESHES_COMMAND");
                        if (deleteSelectedMeshesCommand)
                        {
                            planeMesh = nullptr;
                            deleteSelectedMeshesCommand->execute();
                        }
                    }
                    if (importPlane)
                    {
                        auto *importMeshesCommand = CommandRegistry::instance().getCommand("IMPORT_MESHES_COMMAND");
                        if (importMeshesCommand)
                        {
                            ImportExportMeshesParams importParams;
                            importParams.m_filePathMeshes = "profiler_import_export_" + std::to_string(i);
                            importMeshesCommand->execute(importParams);
                        }
                    }
                }
            }
        }

        ImGui::TreePop();
    }

    ImGui::End();
}

void DebugLayer::updateEdgesVaoData(Mesh *mesh)
{
    Material *defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");
    if (auto opt = mesh->bufferLayout.getLineBufferStorage(defaultLineMaterial))
    {
        LineBufferStorage &lineBufferStorage = opt->get();
        lineBufferStorage.update();
    }
}

void DebugLayer::updateFacesVaoData(Mesh *mesh)
{
    Material *defaultMeshMaterial = MaterialRegistry::getMaterial("defaultMeshMaterial");
    if (auto opt = mesh->bufferLayout.getTriangleBufferStorage(defaultMeshMaterial))
    {
        TriangleBufferStorage &triangleBufferStorage = opt->get();
        triangleBufferStorage.update();
    }
}

void DebugLayer::unhighlightAllFaces(Mesh *mesh)
{
    for (ExtendedFace *face : mesh->m_halfEdgeStructure->m_faces)
    {
        face->m_isSkewed = false;

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
}