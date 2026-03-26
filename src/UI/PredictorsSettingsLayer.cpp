#include "DebugLayer.h"
#include "LayerRegistry.h"

#include "../ViewPortsController.h"
#include "../Core/Layer.h"
#include <vector>
#include <cassert>

#include "Callbacks/SplitEdgeCallback.h"
#include "Renderer/MaterialRegistry.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"
#include "../Ml/Analyser/AnalyserRegistry.h"
#include "Ml/NeuralNetworkModels/FaceSkewnessModel.h"
#include "PredictorsSettingsLayer.h"
#include <algorithm>

static AutoRegisterLayerArgs<PredictorsSettingsLayer, std::string> regPredictorsSettingsLayer;

PredictorsSettingsLayer::PredictorsSettingsLayer(const std::string &name)
    : LayerWithID(name)
{
}

void PredictorsSettingsLayer::onEvent(Event &event)
{
    if (event.getType() == EventType::MouseButtonPress)
    {
        if (m_isMouseInsideWindow)
        {
            event.isHandled = true;
        }
    }
}

void PredictorsSettingsLayer::initializeFeatures(Predictor* predictor)
{
    m_toBeAddedFeatures.clear();
    m_toBeRemovedFeatures.clear();

    m_usedFeatures.clear();
    m_notUsedFeatures.clear();
    m_selectedFeatureToAddIndex = 0;

    if (!predictor) {
        return;
    }

    std::vector<FeatureStrategyConcept*> allFeatures =
        FeatureStrategyRegistry::instance().getFeaturesByType(predictor->getFeatureObjectType());

    std::vector<FeatureStrategyConcept*> predictorFeatures = predictor->getFeatures();

    for (FeatureStrategyConcept* feature : allFeatures)
    {
        auto it = std::find(predictorFeatures.begin(), predictorFeatures.end(), feature);

        if (it != predictorFeatures.end()) {
            m_usedFeatures.push_back(feature);
        } else {
            m_notUsedFeatures.push_back(feature);
        }
    }
}

void PredictorsSettingsLayer::onImGuiRender()
{
    if (!m_initialized)
    {
        m_predictors.clear();
        m_predictors = AnalyserRegistry::instance().getPredictors();

        if (!m_predictors.empty())
        {
            m_selectedPredictorIndex = 0;
            initializeFeatures(m_predictors[m_selectedPredictorIndex]);
        }

        m_previousPredictorIndex = m_selectedPredictorIndex;
        m_initialized = true;
    }


    ImGui::Begin(this->getName().c_str());

    // Get window position and size
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 mousePos = ImGui::GetMousePos();

    // Update the class variable to track if the mouse is inside the window
    m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
        mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);


    ImGui::Text("Choose Predictor:");
    bool predictorChanged = false;

    std::string currentPredictorName =
     m_predictors.empty() ? "None" : m_predictors[m_selectedPredictorIndex]->getName();

    if (ImGui::BeginCombo("##PredictorDropdown", currentPredictorName.c_str()))
    {
        for (int i = 0; i < static_cast<int>(m_predictors.size()); ++i)
        {
            bool isSelected = (m_selectedPredictorIndex == i);

            if (ImGui::Selectable(m_predictors[i]->getName().c_str(), isSelected))
            {
                m_selectedPredictorIndex = i;
                predictorChanged = true;
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (predictorChanged)
    {
        initializeFeatures(m_predictors[m_selectedPredictorIndex]);
    }

    ImGui::Separator();


    ImGui::Text("Used Features:");
    int featureToRemove = -1;

    for (int i = 0; i < static_cast<int>(m_usedFeatures.size()); ++i)
    {
        ImGui::PushID(i);

        std::string featureName = m_usedFeatures[i]->getName();
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
        auto it =
             std::find(m_toBeAddedFeatures.begin(), m_toBeAddedFeatures.end(), m_usedFeatures[featureToRemove]);

        if (it != m_toBeAddedFeatures.end()) {
            m_toBeAddedFeatures.erase(it);
        } else {
            m_toBeRemovedFeatures.push_back(m_usedFeatures[featureToRemove]);
        }

        m_notUsedFeatures.push_back(m_usedFeatures[featureToRemove]);
        m_usedFeatures.erase(m_usedFeatures.begin() + featureToRemove);

        if (m_selectedFeatureToAddIndex >= static_cast<int>(m_notUsedFeatures.size()))
        {
            m_selectedFeatureToAddIndex = m_notUsedFeatures.empty() ? 0 : static_cast<int>(m_notUsedFeatures.size()) - 1;
        }
    }

    ImGui::Separator();

    ImGui::Text("Add Feature:");
    std::string currentFeatureToAddName =
        m_notUsedFeatures.empty() ? "None" : m_notUsedFeatures[m_selectedFeatureToAddIndex]->getName();

    if (ImGui::BeginCombo("##AddFeatureDropdown", currentFeatureToAddName.c_str()))
    {
        for (int i = 0; i < static_cast<int>(m_notUsedFeatures.size()); ++i)
        {
            bool isSelected = (m_selectedFeatureToAddIndex == i);

            if (ImGui::Selectable(m_notUsedFeatures[i]->getName().c_str(), isSelected))
            {
                m_selectedFeatureToAddIndex = i;
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (!m_notUsedFeatures.empty())
    {
        if (ImGui::Button("Add Selected Feature"))
        {
            auto it =
                std::find(m_toBeRemovedFeatures.begin(), m_toBeRemovedFeatures.end(), m_notUsedFeatures[m_selectedFeatureToAddIndex]);

            if (it != m_toBeRemovedFeatures.end()) {
                m_toBeRemovedFeatures.erase(it);
            } else {
                m_toBeAddedFeatures.push_back(m_notUsedFeatures[m_selectedFeatureToAddIndex]);
            }

            m_usedFeatures.push_back(m_notUsedFeatures[m_selectedFeatureToAddIndex]);
            m_notUsedFeatures.erase(m_notUsedFeatures.begin() + m_selectedFeatureToAddIndex);

            if (m_selectedFeatureToAddIndex >= static_cast<int>(m_notUsedFeatures.size()))
            {
                m_selectedFeatureToAddIndex = m_notUsedFeatures.empty() ? 0 : static_cast<int>(m_notUsedFeatures.size()) - 1;
            }
        }
    }

    const bool hasPendingFeatureChanges =
     !m_toBeAddedFeatures.empty() || !m_toBeRemovedFeatures.empty();

    if (hasPendingFeatureChanges)
    {
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.75f, 0.15f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.20f, 0.20f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.60f, 0.10f, 0.10f, 1.0f));

        if (ImGui::Button("Retrain to apply", ImVec2(180.0f, 0.0f)))
        {
            //FACE AREA FEATURE MOZME VYNECHAT ZATIAL
            for (FeatureStrategyConcept* feature: m_toBeRemovedFeatures) {
                m_predictors[m_selectedPredictorIndex]->removeFeature(feature);
            }

            for (FeatureStrategyConcept* feature: m_toBeAddedFeatures) {
                m_predictors[m_selectedPredictorIndex]->addFeature(feature);
            }

            m_toBeRemovedFeatures.clear();
            m_toBeAddedFeatures.clear();

            SelectionController *selectionController = ViewPortsHolderContext::s_selectionController;
            const SelectionHolder &selectionHolder = selectionController->getHolder();
            const std::vector<Mesh *> &selectedMeshes = selectionHolder.meshes;

            m_predictors[m_selectedPredictorIndex]->train(selectedMeshes);
        }

        ImGui::PopStyleColor(3);
    }

    ImGui::End();
}