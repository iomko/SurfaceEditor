#pragma once

#include <string>
#include <vector>

#include "imgui.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include "../../src/Scene/Mesh.h"
#include "../../src/Ml/Analyser/Analyser.h"
#include "../../src/Ml/Analyser/AnalyserRegistry.h"
#include "../../src/Ml/Editor/PredictorEditor.h"

enum class TrainingMeshSourceMode
{
    SceneMeshes = 0,
    ObjFiles = 1
};

class PredictorsSettingsLayer : public Layer, public Observable, public Observer {
public:
    PredictorsSettingsLayer(const std::string& name);

    void onEvent(Event& event) override;
    void onImGuiRender() override;

private:
    Predictor* getSelectedPredictor() const;
    void refreshPredictorInstances();

    void renderTrainingSourceSettings();
    std::vector<Mesh*> collectTrainingMeshes();

    std::vector<Mesh*> collectSceneTrainingMeshes();
    std::vector<Mesh*> collectObjTrainingMeshes();

private:
    bool m_isMouseInsideWindow = false;

    std::vector<AnalyserRegistry::InstanceInfo> m_predictorInstances;

    int m_selectedPredictorIndex = 0;
    int m_previousPredictorIndex = -1;

    PredictorEditor m_predictorEditor;

    bool m_initialized = false;
    char m_predictorNameBuffer[256] = {};

    TrainingMeshSourceMode m_trainingMeshSourceMode = TrainingMeshSourceMode::SceneMeshes;
    bool m_trainOnlySelected = true;

    std::vector<std::string> m_selectedObjFilePaths;
};