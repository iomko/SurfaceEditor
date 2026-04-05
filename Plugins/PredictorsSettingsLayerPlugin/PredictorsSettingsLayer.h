#pragma once

#include <string>
#include <vector>

#include "imgui.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include "../../src/Ml/Analyser/Analyser.h"
#include "../../src/Ml/Editor/PredictorEditor.h"

class PredictorsSettingsLayer : public Layer, public Observable, public Observer {
public:
    PredictorsSettingsLayer(const std::string& name);

    void onEvent(Event& event) override;
    void onImGuiRender() override;

private:
    Predictor* getSelectedPredictor() const;

private:
    bool m_isMouseInsideWindow = false;

    std::vector<std::string> m_predictorNames;
    std::vector<std::string> m_predictorIds;

    int m_selectedPredictorIndex = 0;
    int m_previousPredictorIndex = -1;

    PredictorEditor m_predictorEditor;

    bool m_initialized = false;
};