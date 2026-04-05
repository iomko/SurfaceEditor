#pragma once

#include <string>
#include <vector>

#include "../Analyser/Analyser.h"

#include "PredictorConfig.h"
#include "FeatureEditor.h"
#include "LabelEditor.h"
#include "PredictorConfigDeserializer.h"
#include "PredictorConfigSerializer.h"

class PredictorEditor
{
public:
    void open(Predictor* predictor)
    {
        m_predictor = predictor;
        m_config = {};

        if (!m_predictor) {
            return;
        }

        m_config = PredictorConfigDeserializer::loadFromFile(getConfigPath());

        if (m_config.featureIds.empty())
        {
            const std::vector<FeatureStrategyConcept*> currentFeatures = m_predictor->getFeatures();
            for (FeatureStrategyConcept* feature : currentFeatures)
            {
                if (feature) {
                    m_config.featureIds.push_back(feature->getId());
                }
            }
        }

        m_featureEditor.initialize(m_predictor, m_config);
        m_labelEditor.initialize(m_predictor, m_config);
    }

    void save()
    {
        if (!m_predictor) {
            return;
        }

        m_config = buildConfig();
        PredictorConfigSerializer::saveToFile(getConfigPath(), m_config);
    }

    Predictor* getPredictor() const
    {
        return m_predictor;
    }

    const PredictorConfig& getConfig() const
    {
        return m_config;
    }

    PredictorConfig buildConfig() const
    {
        PredictorConfig config;
        config.featureIds = m_featureEditor.collectSelectedFeatureIds();
        config.labels = m_labelEditor.collectLabels();
        return config;
    }

    FeatureEditor& getFeatureEditor()
    {
        return m_featureEditor;
    }

    const FeatureEditor& getFeatureEditor() const
    {
        return m_featureEditor;
    }

    LabelEditor& getLabelEditor()
    {
        return m_labelEditor;
    }

    const LabelEditor& getLabelEditor() const
    {
        return m_labelEditor;
    }

    void applyFeatureChanges()
    {
        if (!m_predictor) {
            return;
        }

        m_featureEditor.applyToPredictor(m_predictor);
    }

private:
    std::string getConfigPath() const
    {
        if (!m_predictor) {
            return {};
        }

        return m_predictor->getModelConfigPath();
    }

private:
    Predictor* m_predictor = nullptr;
    PredictorConfig m_config;

    FeatureEditor m_featureEditor;
    LabelEditor m_labelEditor;
};