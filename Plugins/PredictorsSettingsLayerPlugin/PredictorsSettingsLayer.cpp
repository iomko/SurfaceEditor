#include "../../src/UI/LayerRegistry.h"

#include "../../src/ViewPortsController.h"
#include "../../src/Core/Layer.h"
#include "../../src/Renderer/MaterialRegistry.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/Commands/CommandIDs.h"
#include "../../src/Ml/Analyser/AnalyserRegistry.h"

#include "PredictorsSettingsLayer.h"

#include <cstring>

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

Predictor* PredictorsSettingsLayer::getSelectedPredictor() const
{
    if (m_selectedPredictorIndex < 0 ||
        m_selectedPredictorIndex >= static_cast<int>(m_predictorIds.size()))
    {
        return nullptr;
    }

    Analyser* analyser =
        AnalyserRegistry::instance().getAnalyser(m_predictorIds[m_selectedPredictorIndex]);

    if (!analyser) {
        return nullptr;
    }

    return dynamic_cast<Predictor*>(analyser);
}

void PredictorsSettingsLayer::onImGuiRender()
{
    if (!m_initialized)
    {
        m_predictorNames.clear();
        m_predictorIds.clear();

        const auto namesWithIds = AnalyserRegistry::instance().getNamesWithId();
        for (const auto& [name, id] : namesWithIds)
        {
            Analyser* analyser = AnalyserRegistry::instance().getAnalyser(id);
            if (!analyser) {
                continue;
            }

            auto* predictor = dynamic_cast<Predictor*>(analyser);
            if (!predictor) {
                continue;
            }

            m_predictorNames.push_back(name);
            m_predictorIds.push_back(id);
        }

        if (!m_predictorIds.empty())
        {
            m_selectedPredictorIndex = 0;
            m_predictorEditor.open(getSelectedPredictor());
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
        m_predictorNames.empty() ? "None" : m_predictorNames[m_selectedPredictorIndex];

    if (ImGui::BeginCombo("##PredictorDropdown", currentPredictorName.c_str()))
    {
        for (int i = 0; i < static_cast<int>(m_predictorNames.size()); ++i)
        {
            const bool isSelected = (m_selectedPredictorIndex == i);

            if (ImGui::Selectable(m_predictorNames[i].c_str(), isSelected))
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

                ImGui::SameLine();

                if (ImGui::Button("Save Config"))
                {
                    m_predictorEditor.save();
                }
            }
            else
            {
                ImGui::TextUnformatted("No label selected.");
            }

            ImGui::EndChild();
        }

        ImGui::Separator();

        if (ImGui::Button("Retrain to apply", ImVec2(180.0f, 0.0f)))
        {
            m_predictorEditor.applyFeatureChanges();
            m_predictorEditor.save();

            SelectionController* selectionController = ViewPortsHolderContext::s_selectionController;
            const SelectionHolder& selectionHolder = selectionController->getHolder();
            const std::vector<Mesh*>& selectedMeshes = selectionHolder.meshes;

            predictor->train(selectedMeshes);
        }
    }

    ImGui::End();
}