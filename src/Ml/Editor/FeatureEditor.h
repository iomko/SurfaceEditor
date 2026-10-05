#pragma once

#include <vector>
#include <algorithm>

#include "../Analyser/Analyser.h"
#include "../FeatureStrategies/FeatureStrategy.h"
#include "../FeatureStrategies/FeatureStrategyRegistry.h"

#include "PredictorConfig.h"
#include "FeatureEditorState.h"

class FeatureEditor
{
public:
    void initialize(Predictor* predictor, const PredictorConfig& config)
    {
        rebuildState(predictor, config);
    }

    FeatureEditorState& getState()
    {
        return m_state;
    }

    const FeatureEditorState& getState() const
    {
        return m_state;
    }

    void addFeature(FeatureStrategyConcept* feature)
    {
        if (!feature) {
            return;
        }

        auto itNotUsed = std::find(m_state.notUsedFeatures.begin(), m_state.notUsedFeatures.end(), feature);
        if (itNotUsed == m_state.notUsedFeatures.end()) {
            return;
        }

        auto itRemoved = std::find(m_state.toBeRemovedFeatures.begin(), m_state.toBeRemovedFeatures.end(), feature);
        if (itRemoved != m_state.toBeRemovedFeatures.end()) {
            m_state.toBeRemovedFeatures.erase(itRemoved);
        } else {
            m_state.toBeAddedFeatures.push_back(feature);
        }

        m_state.usedFeatures.push_back(feature);
        m_state.notUsedFeatures.erase(itNotUsed);

        if (m_state.selectedFeatureToAddIndex >= static_cast<int>(m_state.notUsedFeatures.size()))
        {
            m_state.selectedFeatureToAddIndex =
                m_state.notUsedFeatures.empty() ? 0 : static_cast<int>(m_state.notUsedFeatures.size()) - 1;
        }
    }

    void removeFeature(FeatureStrategyConcept* feature)
    {
        if (!feature) {
            return;
        }

        auto itUsed = std::find(m_state.usedFeatures.begin(), m_state.usedFeatures.end(), feature);
        if (itUsed == m_state.usedFeatures.end()) {
            return;
        }

        auto itAdded = std::find(m_state.toBeAddedFeatures.begin(), m_state.toBeAddedFeatures.end(), feature);
        if (itAdded != m_state.toBeAddedFeatures.end()) {
            m_state.toBeAddedFeatures.erase(itAdded);
        } else {
            m_state.toBeRemovedFeatures.push_back(feature);
        }

        m_state.notUsedFeatures.push_back(feature);
        m_state.usedFeatures.erase(itUsed);

        if (m_state.selectedFeatureToAddIndex >= static_cast<int>(m_state.notUsedFeatures.size()))
        {
            m_state.selectedFeatureToAddIndex =
                m_state.notUsedFeatures.empty() ? 0 : static_cast<int>(m_state.notUsedFeatures.size()) - 1;
        }
    }

    std::vector<int> collectSelectedFeatureIds() const
    {
        std::vector<int> featureIds;
        featureIds.reserve(m_state.usedFeatures.size());

        for (FeatureStrategyConcept* feature : m_state.usedFeatures)
        {
            if (feature) {
                featureIds.push_back(feature->getId());
            }
        }

        return featureIds;
    }

    void applyToPredictor(Predictor* predictor) const
    {
        if (!predictor) {
            return;
        }

        for (FeatureStrategyConcept* feature : m_state.toBeRemovedFeatures) {
            predictor->removeFeature(feature);
        }

        for (FeatureStrategyConcept* feature : m_state.toBeAddedFeatures) {
            predictor->addFeature(feature);
        }
    }

private:
    void rebuildState(Predictor* predictor, const PredictorConfig& config)
    {
        m_state = {};

        if (!predictor) {
            return;
        }

        std::vector<FeatureStrategyConcept*> allFeatures =
            FeatureStrategyRegistry::instance().getFeaturesByType(predictor->getFeatureObjectType());

        for (FeatureStrategyConcept* feature : allFeatures)
        {
            if (!feature) {
                continue;
            }

            const bool isUsed =
                std::find(config.featureIds.begin(), config.featureIds.end(), feature->getId()) != config.featureIds.end();

            if (isUsed) {
                m_state.usedFeatures.push_back(feature);
            } else {
                m_state.notUsedFeatures.push_back(feature);
            }
        }
    }

private:
    FeatureEditorState m_state;
};