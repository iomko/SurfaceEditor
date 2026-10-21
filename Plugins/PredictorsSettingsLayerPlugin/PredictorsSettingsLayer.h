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
#include <Components/CollapsingHeader.h>
#include <Components/Combo.h>
#include <Components/ChildWindow.h>
#include <Components/InputBox.h>
#include <Components/CheckBox.h>

enum class TrainingMeshSourceMode
{
    SceneMeshes = 0,
    ObjFiles = 1
};

class PredictorsSettingsLayer : public Layer, public Observable, public Observer, public ui::components::Window {
public:
    PredictorsSettingsLayer(const std::string& name);

    void onEvent(Event& event) override;
    void onImGuiRender() override;

protected:
    void initWindowConfig() override;

    void initComponents() override;


private:
    Predictor* getSelectedPredictor() const;
    void refreshPredictorInstances();

    void renderTrainingSourceSettings(ui::components::CheckBox& trainOnlySelectedCheckBox);
    std::vector<Mesh*> collectTrainingMeshes();

    std::vector<Mesh*> collectSceneTrainingMeshes();
    std::vector<Mesh*> collectObjTrainingMeshes();

private:
    bool m_isMouseInsideWindow = false;

    std::vector<AnalyserRegistry::InstanceInfo> m_predictorInstances;

    int m_selectedPredictorIndex = 0;
    int m_previousPredictorIndex = -1;

    PredictorEditor m_predictorEditor;
    ui::components::ChildWindow* m_predictorContent = nullptr;
    ui::components::InputBox<char>* m_predictorNameInputBox = nullptr;

    bool m_initialized = false;
    char m_predictorNameBuffer[256] = {};

    TrainingMeshSourceMode m_trainingMeshSourceMode = TrainingMeshSourceMode::SceneMeshes;
    bool m_trainOnlySelected = true;
    std::vector<std::string> m_trainingSourceOptions{"Meshes From Scene", "OBJ Files"};

    std::vector<std::string> m_selectedObjFilePaths;
};