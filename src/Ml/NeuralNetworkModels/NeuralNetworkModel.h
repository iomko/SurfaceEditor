#pragma once

#include "../Analyser/Analyser.h"
#include "../FeatureStrategies/FeatureStrategyRegistry.h"
#include "../Editor/PredictorConfig.h"
#include "../Editor/PredictorConfigDeserializer.h"
#include "../Editor/PredictorConfigSerializer.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct ModelInfo {
    size_t featuresSize = static_cast<size_t>(-1);
};

template<typename Type>
class NeuralNetworkModel : public Predictor {
public:
    NeuralNetworkModel(const std::string& modelName, const std::vector<int>& defaultFeatureIds) :
        m_modelName(modelName), m_defaultFeatureIds(defaultFeatureIds)
    {
        initializeFeaturesWithConfig();
    }

    void setModelInfo(const ModelInfo& modelInfo) {
        m_info.featuresSize = modelInfo.featuresSize;
    }

    ModelInfo getModelInfo() {
        return m_info;
    }

    FeatureObjectType getFeatureObjectType() const override {
        return FeatureObjectTypeTrait<Type>::value;
    }

    bool addFeature(FeatureStrategyConcept* feature) override {
        if (!feature) {
            return false;
        }

        if (feature->getObjectType() != getFeatureObjectType()) {
            return false;
        }

        auto* typedFeature = dynamic_cast<FeatureStrategy<Type>*>(feature);
        if (!typedFeature) {
            return false;
        }

        auto it = std::find(m_featureStrategies.begin(), m_featureStrategies.end(), typedFeature);
        if (it != m_featureStrategies.end()) {
            return false;
        }

        m_featureStrategies.push_back(typedFeature);

        saveFeatureIdsToConfig();
        rebuildModelInfo();
        return true;
    }

    bool removeFeature(FeatureStrategyConcept* feature) override {
        if (!feature) {
            return false;
        }

        if (feature->getObjectType() != getFeatureObjectType()) {
            return false;
        }

        auto* typedFeature = dynamic_cast<FeatureStrategy<Type>*>(feature);
        if (!typedFeature) {
            return false;
        }

        auto it = std::find(m_featureStrategies.begin(), m_featureStrategies.end(), typedFeature);
        if (it == m_featureStrategies.end()) {
            return false;
        }

        m_featureStrategies.erase(it);

        saveFeatureIdsToConfig();
        rebuildModelInfo();
        return true;
    }

    std::vector<FeatureStrategyConcept*> getFeatures() const override {
        return {
            m_featureStrategies.begin(),
            m_featureStrategies.end()
        };
    }

    std::string getModelConfigPath() const override {
        return std::string(ML_DATA_DIR) + "/" + m_modelName + ".config.json";
    }

    std::string getName() const override {
        return m_modelName;
    }

protected:
    ModelInfo m_info;
    std::vector<FeatureStrategy<Type>*> m_featureStrategies;

private:
    std::string m_modelName;
    std::vector<int> m_defaultFeatureIds;

    void rebuildModelInfo()
    {
        size_t featuresSize = 0;
        for (auto* featureStrategy : m_featureStrategies) {
            featuresSize += featureStrategy->getSize();
        }

        m_info.featuresSize = featuresSize;
    }

    PredictorConfig makeDefaultConfig() const
    {
        PredictorConfig config;
        config.featureIds = m_defaultFeatureIds;
        return config;
    }

    PredictorConfig loadOrCreateConfig()
    {
        PredictorConfig config = PredictorConfigDeserializer::loadFromFile(getModelConfigPath());

        if (config.featureIds.empty()) {
            config = makeDefaultConfig();
            PredictorConfigSerializer::saveToFile(getModelConfigPath(), config);
        }

        return config;
    }

    void initializeFeaturesWithConfig()
    {
        PredictorConfig config = loadOrCreateConfig();

        m_featureStrategies.clear();

        for (int id : config.featureIds)
        {
            FeatureStrategyConcept* feature =
                FeatureStrategyRegistry::instance().getFeature(id);

            if (!feature) {
                std::cerr << "[ML] WARNING: Feature ID " << id << " not found.\n";
                continue;
            }

            if (feature->getObjectType() != getFeatureObjectType()) {
                std::cerr << "[ML] WARNING: Feature ID " << id
                          << " has incompatible object type for model " << m_modelName << ".\n";
                continue;
            }

            auto* typedFeature = dynamic_cast<FeatureStrategy<Type>*>(feature);
            if (!typedFeature) {
                std::cerr << "[ML] WARNING: Failed to cast feature ID " << id << ".\n";
                continue;
            }

            auto it = std::find(m_featureStrategies.begin(), m_featureStrategies.end(), typedFeature);
            if (it == m_featureStrategies.end()) {
                m_featureStrategies.push_back(typedFeature);
            }
        }

        rebuildModelInfo();
    }

    void saveFeatureIdsToConfig() const
    {
        PredictorConfig config = PredictorConfigDeserializer::loadFromFile(getModelConfigPath());

        config.featureIds.clear();
        for (auto* feature : m_featureStrategies)
        {
            if (feature) {
                config.featureIds.push_back(feature->getId());
            }
        }

        PredictorConfigSerializer::saveToFile(getModelConfigPath(), config);
    }
};