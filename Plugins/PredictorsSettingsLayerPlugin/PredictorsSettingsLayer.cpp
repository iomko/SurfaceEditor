#include "../../src/UI/LayerRegistry.h"

#include "../../src/ViewPortsController.h"
#include "../../src/Core/Layer.h"
#include "../../src/Renderer/MaterialRegistry.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/Commands/CommandIDs.h"
#include "../../src/Ml/Analyser/AnalyserRegistry.h"

#include "PredictorsSettingsLayer.h"

#include "ImGuiFileDialog.h"

#include "../ImportMeshesPlugin/OBJImporter.h"

#include <cstring>
#include <iostream>

static AutoRegisterLayerArgs<PredictorsSettingsLayer, std::string> reg("PREDICTORS_SETTINGS_LAYER");

PredictorsSettingsLayer::PredictorsSettingsLayer(const std::string& name)
    : Layer(name)
{
}

void PredictorsSettingsLayer::onEvent(Event& event)
{
    if (event.getType() == EventType::MouseButtonPress)
    {
        if (m_isMouseInsideWindow)
        {
            event.isHandled = true;
        }
    }
}

void PredictorsSettingsLayer::refreshPredictorInstances()
{
    m_predictorInstances = AnalyserRegistry::instance().getPredictorInstances();
}

Predictor* PredictorsSettingsLayer::getSelectedPredictor() const
{
    if (m_selectedPredictorIndex < 0 ||
        m_selectedPredictorIndex >= static_cast<int>(m_predictorInstances.size()))
    {
        return nullptr;
    }

    Analyser* analyser =
        AnalyserRegistry::instance().getAnalyserInstance(m_predictorInstances[m_selectedPredictorIndex].instanceId);

    if (!analyser) {
        return nullptr;
    }

    return dynamic_cast<Predictor*>(analyser);
}

void PredictorsSettingsLayer::renderTrainingSourceSettings()
{
    ImGui::Separator();
    ImGui::Text("Training Mesh Source");

    int sourceMode = static_cast<int>(m_trainingMeshSourceMode);
    const char* sourceModes[] = { "Meshes From Scene", "OBJ Files" };

    if (ImGui::Combo("Source", &sourceMode, sourceModes, IM_ARRAYSIZE(sourceModes)))
    {
        m_trainingMeshSourceMode = static_cast<TrainingMeshSourceMode>(sourceMode);
    }

    if (m_trainingMeshSourceMode == TrainingMeshSourceMode::SceneMeshes)
    {
        ImGui::Checkbox("Train only on selected", &m_trainOnlySelected);

        if (m_trainOnlySelected)
        {
            ImGui::TextDisabled("Training will use only selected meshes.");
        }
        else
        {
            ImGui::TextDisabled("Training will use all meshes in the scene.");
        }
    }
    else if (m_trainingMeshSourceMode == TrainingMeshSourceMode::ObjFiles)
    {
        if (ImGui::Button("Select OBJ Files", ImVec2(180.0f, 0.0f)))
        {
            IGFD::FileDialogConfig config;
            config.path = ".";
            config.countSelectionMax = 0;
            config.flags = 0;

            ImGuiFileDialog::Instance()->OpenDialog(
                "ChooseObjFilesDialog",
                "Choose OBJ Files",
                ".obj",
                config
            );
        }

        if (ImGuiFileDialog::Instance()->Display("ChooseObjFilesDialog"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                m_selectedObjFilePaths.clear();

                const auto selection = ImGuiFileDialog::Instance()->GetSelection();

                for (const auto& selectedFile : selection)
                {
                    m_selectedObjFilePaths.push_back(selectedFile.second);
                }
            }

            ImGuiFileDialog::Instance()->Close();
        }

        ImGui::Text("Selected OBJ files: %d", static_cast<int>(m_selectedObjFilePaths.size()));

        ImGui::BeginChild("SelectedObjFilesList", ImVec2(0.0f, 90.0f), true);

        if (m_selectedObjFilePaths.empty())
        {
            ImGui::TextDisabled("No OBJ files selected.");
        }
        else
        {
            for (const std::string& filePath : m_selectedObjFilePaths)
            {
                ImGui::BulletText("%s", filePath.c_str());
            }
        }

        ImGui::EndChild();

        if (!m_selectedObjFilePaths.empty())
        {
            if (ImGui::Button("Clear OBJ Selection", ImVec2(180.0f, 0.0f)))
            {
                m_selectedObjFilePaths.clear();
            }
        }
    }
}

std::vector<Mesh*> PredictorsSettingsLayer::collectSceneTrainingMeshes()
{
    std::vector<Mesh*> meshes;

    if (m_trainOnlySelected)
    {
        SelectionController* selectionController = ViewPortsHolderContext::s_selectionController;

        if (!selectionController)
        {
            std::cerr << "[ML] ERROR: SelectionController is null." << std::endl;
            return meshes;
        }

        const SelectionHolder& selectionHolder = selectionController->getHolder();
        meshes = selectionHolder.meshes;
    }
    else
    {
        SceneResources::MeshFaceOctreeCoordsMap meshFacesPairMap = ViewPortsHolderContext::s_viewPortsController->m_scene->m_res.meshFaceOctreeCoordsMap;

        for (auto& [mesh, _] : meshFacesPairMap) {
            meshes.emplace_back(mesh);
        }
    }

    return meshes;
}

std::vector<Mesh*> PredictorsSettingsLayer::collectObjTrainingMeshes()
{
    std::vector<Mesh*> meshes;

    if (m_selectedObjFilePaths.empty())
    {
        std::cerr << "[ML] ERROR: No OBJ files selected." << std::endl;
        return meshes;
    }

    for (const std::string& filePath : m_selectedObjFilePaths) {

        OBJImporter objImporter;
        objImporter.setFilePath(filePath);
        objImporter.read();

        for (Mesh* mesh : objImporter.getMeshes()) {
            
            if(mesh) {
                meshes.emplace_back(mesh);
            }

        }
    }

    return meshes;
}

std::vector<Mesh*> PredictorsSettingsLayer::collectTrainingMeshes()
{
    if (m_trainingMeshSourceMode == TrainingMeshSourceMode::SceneMeshes)
    {
        return collectSceneTrainingMeshes();
    }

    if (m_trainingMeshSourceMode == TrainingMeshSourceMode::ObjFiles)
    {
        return collectObjTrainingMeshes();
    }

    return {};
}

void PredictorsSettingsLayer::onImGuiRender()
{
    if (!m_initialized)
    {
        refreshPredictorInstances();

        if (!m_predictorInstances.empty())
        {
            m_selectedPredictorIndex = 0;
            m_predictorEditor.open(getSelectedPredictor());

            Predictor* predictor = getSelectedPredictor();
            if (predictor) {
                std::memset(m_predictorNameBuffer, 0, sizeof(m_predictorNameBuffer));
                std::strncpy(m_predictorNameBuffer, predictor->getName().c_str(), sizeof(m_predictorNameBuffer) - 1);
            }
        }

        m_previousPredictorIndex = m_selectedPredictorIndex;
        m_initialized = true;
    }

    ImGui::Begin(this->getName().c_str());

    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 mousePos = ImGui::GetMousePos();

    m_isMouseInsideWindow =
        (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
         mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

    ImGui::Text("Choose Predictor:");
    bool predictorChanged = false;

    const std::string currentPredictorName =
        m_predictorInstances.empty() ? "None" : m_predictorInstances[m_selectedPredictorIndex].displayName;

    if (ImGui::BeginCombo("##PredictorDropdown", currentPredictorName.c_str()))
    {
        for (int i = 0; i < static_cast<int>(m_predictorInstances.size()); ++i)
        {
            const bool isSelected = (m_selectedPredictorIndex == i);

            if (ImGui::Selectable(m_predictorInstances[i].displayName.c_str(), isSelected))
            {
                m_selectedPredictorIndex = i;
                predictorChanged = true;
            }

            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    if (predictorChanged)
    {
        m_predictorEditor.open(getSelectedPredictor());

        Predictor* predictor = getSelectedPredictor();
        std::memset(m_predictorNameBuffer, 0, sizeof(m_predictorNameBuffer));
        if (predictor) {
            std::strncpy(m_predictorNameBuffer, predictor->getName().c_str(), sizeof(m_predictorNameBuffer) - 1);
        }
    }

    Predictor* predictor = m_predictorEditor.getPredictor();
    if (predictor)
    {
        FeatureEditor& featureEditor = m_predictorEditor.getFeatureEditor();
        LabelEditor& labelEditor = m_predictorEditor.getLabelEditor();

        FeatureEditorState& featureState = featureEditor.getState();
        LabelEditorState& labelState = labelEditor.getState();

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Feature Editor", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Used Features:");
            int featureToRemove = -1;

            for (int i = 0; i < static_cast<int>(featureState.usedFeatures.size()); ++i)
            {
                ImGui::PushID(i);

                const std::string featureName = featureState.usedFeatures[i]->getName();
                ImGui::Text("%s", featureName.c_str());

                ImGui::SameLine(250.0f);
                if (ImGui::Button("Remove"))
                {
                    featureToRemove = i;
                }

                ImGui::PopID();
            }

            if (featureToRemove != -1)
            {
                featureEditor.removeFeature(featureState.usedFeatures[featureToRemove]);
            }

            ImGui::Separator();
            ImGui::Text("Add Feature:");

            std::string currentFeatureToAddName =
                featureState.notUsedFeatures.empty() ? "None" : featureState.notUsedFeatures[featureState.selectedFeatureToAddIndex]->getName();

            if (ImGui::BeginCombo("##AddFeatureDropdown", currentFeatureToAddName.c_str()))
            {
                for (int i = 0; i < static_cast<int>(featureState.notUsedFeatures.size()); ++i)
                {
                    const bool isSelected = (featureState.selectedFeatureToAddIndex == i);

                    if (ImGui::Selectable(featureState.notUsedFeatures[i]->getName().c_str(), isSelected))
                    {
                        featureState.selectedFeatureToAddIndex = i;
                    }

                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            if (!featureState.notUsedFeatures.empty())
            {
                if (ImGui::Button("Add Selected Feature"))
                {
                    featureEditor.addFeature(featureState.notUsedFeatures[featureState.selectedFeatureToAddIndex]);
                }
            }
        }

        if (ImGui::CollapsingHeader("Label Editor", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Separator();
            ImGui::Text("Label Editor");

            ImGui::BeginChild("LabelsList", ImVec2(220.0f, 260.0f), true);

            for (int i = 0; i < static_cast<int>(labelState.labels.size()); ++i)
            {
                const bool selected = (labelState.selectedLabelIndex == i);
                if (ImGui::Selectable(labelState.labels[i].name.c_str(), selected))
                {
                    labelState.selectedLabelIndex = i;
                }
            }

            if (ImGui::Button("+ Add Label"))
            {
                labelEditor.addLabel();
            }

            if (labelState.selectedLabelIndex >= 0 &&
                labelState.selectedLabelIndex < static_cast<int>(labelState.labels.size()))
            {
                if (ImGui::Button("Delete Label"))
                {
                    labelEditor.removeSelectedLabel();
                }
            }

            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("SelectedLabelEditor", ImVec2(0.0f, 260.0f), true);

            LabelDefinition* label = labelEditor.getSelectedLabel();
            if (label)
            {
                char nameBuffer[256];
                std::memset(nameBuffer, 0, sizeof(nameBuffer));
                std::strncpy(nameBuffer, label->name.c_str(), sizeof(nameBuffer) - 1);

                if (ImGui::InputText("Label Name", nameBuffer, sizeof(nameBuffer)))
                {
                    label->name = nameBuffer;
                }

                int mode = static_cast<int>(label->mode);
                const char* modes[] = { "ALL", "ANY" };
                if (ImGui::Combo("Match Mode", &mode, modes, IM_ARRAYSIZE(modes)))
                {
                    label->mode = static_cast<LabelMatchMode>(mode);
                }

                ImGui::Separator();
                ImGui::Text("Conditions");

                int conditionToRemove = -1;

                for (int i = 0; i < static_cast<int>(label->conditions.size()); ++i)
                {
                    ImGui::PushID(i);

                    LabelCondition& cond = label->conditions[i];

                    int selectedPropertyIndex = -1;
                    for (int p = 0; p < static_cast<int>(labelState.propertyOptions.size()); ++p)
                    {
                        if (labelState.propertyOptions[p].featureId == cond.featureId &&
                            labelState.propertyOptions[p].componentIndex == cond.componentIndex)
                        {
                            selectedPropertyIndex = p;
                            break;
                        }
                    }

                    const char* currentPropertyName =
                        (selectedPropertyIndex >= 0 && selectedPropertyIndex < static_cast<int>(labelState.propertyOptions.size()))
                            ? labelState.propertyOptions[selectedPropertyIndex].displayName.c_str()
                            : "None";

                    if (ImGui::BeginCombo("Property", currentPropertyName))
                    {
                        for (int p = 0; p < static_cast<int>(labelState.propertyOptions.size()); ++p)
                        {
                            const bool selected = (selectedPropertyIndex == p);

                            if (ImGui::Selectable(labelState.propertyOptions[p].displayName.c_str(), selected))
                            {
                                cond.featureId = labelState.propertyOptions[p].featureId;
                                cond.componentIndex = labelState.propertyOptions[p].componentIndex;
                            }

                            if (selected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }

                        ImGui::EndCombo();
                    }

                    int op = static_cast<int>(cond.op);
                    const char* ops[] = { "==", "!=", "<", "<=", ">", ">=" };
                    if (ImGui::Combo("Operator", &op, ops, IM_ARRAYSIZE(ops)))
                    {
                        cond.op = static_cast<LabelOperator>(op);
                    }

                    ImGui::DragFloat("Value", &cond.value, 0.01f);

                    if (ImGui::Button("Remove Condition"))
                    {
                        conditionToRemove = i;
                    }

                    ImGui::Separator();
                    ImGui::PopID();
                }

                if (conditionToRemove != -1)
                {
                    labelEditor.removeConditionFromSelectedLabel(conditionToRemove);
                }

                if (ImGui::Button("+ Add Condition"))
                {
                    labelEditor.addConditionToSelectedLabel();
                }
            }
            else
            {
                ImGui::TextUnformatted("No label selected.");
            }

            ImGui::EndChild();
        }

        ImGui::Separator();
        ImGui::InputText("Predictor Name", m_predictorNameBuffer, sizeof(m_predictorNameBuffer));

        renderTrainingSourceSettings();

        if (ImGui::Button("Retrain Current", ImVec2(180.0f, 0.0f)))
        {
            Predictor* currentPredictor = m_predictorEditor.getPredictor();

            if (currentPredictor && std::strlen(m_predictorNameBuffer) > 0)
            {
                const std::string typedPredictorName = m_predictorNameBuffer;
                const std::string currentInstanceName = currentPredictor->getInstanceName();

                if (typedPredictorName == currentInstanceName)
                {
                    std::vector<Mesh*> trainingMeshes = collectTrainingMeshes();

                    if (trainingMeshes.empty())
                    {
                        std::cerr << "[ML] ERROR: Cannot retrain predictor. No training meshes were found." << std::endl;
                    }
                    else
                    {
                        m_predictorEditor.applyFeatureChanges();
                        m_predictorEditor.save();

                        currentPredictor->reloadFromConfig();
                        currentPredictor->train(trainingMeshes);
                    }
                }
                else
                {
                    std::cerr << "[ML] ERROR: Cannot retrain current predictor. Predictor Name '"
                              << typedPredictorName
                              << "' does not match the currently selected predictor '"
                              << currentInstanceName << "'." << std::endl;
                }
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Train As New Copy", ImVec2(180.0f, 0.0f)))
        {
            Predictor* currentPredictor = m_predictorEditor.getPredictor();

            if (currentPredictor && std::strlen(m_predictorNameBuffer) > 0)
            {
                const std::string newInstanceName = m_predictorNameBuffer;
                const std::string currentInstanceName = currentPredictor->getInstanceName();

                bool predictorAlreadyExists = false;
                for (const auto& predictorInstance : m_predictorInstances)
                {
                    if (predictorInstance.instanceId == newInstanceName)
                    {
                        predictorAlreadyExists = true;
                        break;
                    }
                }

                if (newInstanceName == currentInstanceName)
                {
                    std::cerr << "[ML] ERROR: Cannot create predictor copy. Predictor Name '"
                              << newInstanceName
                              << "' matches the currently selected predictor. Use Retrain Current instead."
                              << std::endl;
                }
                else if (predictorAlreadyExists)
                {
                    std::cerr << "[ML] ERROR: Cannot create predictor copy. Predictor with name '"
                              << newInstanceName << "' already exists." << std::endl;
                }
                else
                {
                    std::vector<Mesh*> trainingMeshes = collectTrainingMeshes();

                    if (trainingMeshes.empty())
                    {
                        std::cerr << "[ML] ERROR: Cannot train new predictor copy. No training meshes were found." << std::endl;
                    }
                    else
                    {
                        const std::string typeId =
                            m_predictorInstances[m_selectedPredictorIndex].typeId;

                        Analyser* analyser =
                            AnalyserRegistry::instance().createAnalyserInstance(typeId, newInstanceName);

                        auto* newPredictor = dynamic_cast<Predictor*>(analyser);
                        if (newPredictor)
                        {
                            m_predictorEditor.saveToPredictor(newPredictor);
                            newPredictor->reloadFromConfig();
                            newPredictor->train(trainingMeshes);

                            refreshPredictorInstances();

                            for (int i = 0; i < static_cast<int>(m_predictorInstances.size()); ++i)
                            {
                                if (m_predictorInstances[i].instanceId == newInstanceName)
                                {
                                    m_selectedPredictorIndex = i;
                                    break;
                                }
                            }

                            m_predictorEditor.open(newPredictor);

                            std::memset(m_predictorNameBuffer, 0, sizeof(m_predictorNameBuffer));
                            std::strncpy(m_predictorNameBuffer, newPredictor->getName().c_str(), sizeof(m_predictorNameBuffer) - 1);
                        }
                    }
                }
            }
        }
    }

    ImGui::End();
}