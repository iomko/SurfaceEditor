#pragma once
#include <string>
#include "imgui.h"
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "LayerIDs.h"
#include "../Ml/Analyser/Analyser.h"

class PredictorsSettingsLayer : public LayerWithID<PREDICTORS_SETTINGS_LAYER>, public Observable, public Observer {
public:
    PredictorsSettingsLayer(const std::string& name);
    void onEvent(Event& event) override;
    void onImGuiRender() override;
private:

    void initializeFeatures(Predictor* predictor);

    bool m_isMouseInsideWindow;

    std::vector<Predictor*> m_predictors;
    int m_selectedPredictorIndex = 0;
    int m_previousPredictorIndex = -1;

    std::vector<FeatureStrategyConcept*> m_usedFeatures;
    std::vector<FeatureStrategyConcept*> m_notUsedFeatures;
    int m_selectedFeatureToAddIndex = 0;

    std::vector<FeatureStrategyConcept*> m_toBeAddedFeatures;
    std::vector<FeatureStrategyConcept*> m_toBeRemovedFeatures;

    bool m_initialized = false;
};
