#pragma once

#include "../Analyser/Analyser.h"
#include "../FeatureStrategies/FeatureStrategyRegistry.h"

struct ModelInfo {
    size_t featuresSize = -1;
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

    std::string getFeatureConfigPath() const override {
        return std::string(ML_DATA_DIR) + "/" + m_modelName + ".features.cfg";
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

    void initializeFeaturesWithConfig()
    {
        std::vector<int> featureIds;

        std::ifstream in(getFeatureConfigPath());

        if (in.is_open())
        {
            //config exists

            std::string line;
            while (std::getline(in, line))
            {
                if (line.empty() || line[0] == '#') {
                    continue;
                }

                std::istringstream iss(line);
                int id;
                if (iss >> id) {
                    featureIds.push_back(id);
                }
            }

            std::cout << "[ML] Loaded feature config: " << getFeatureConfigPath() << "\n";
        }
        else
        {
            //config doesnt exist
            std::cout << "[ML] Config not found. Creating default config: "
                      << getFeatureConfigPath() << "\n";

            featureIds = m_defaultFeatureIds;

            std::ofstream out(getFeatureConfigPath(), std::ios::out | std::ios::trunc);
            if (!out.is_open()) {
                std::cerr << "[ML] ERROR: Cannot create config file.\n";
            }
            else {
                out << "# Auto-generated feature config\n";
                for (int id : featureIds) {
                    out << id << "\n";
                }
            }
        }

        m_featureStrategies.clear();
        for (int id : featureIds)
        {
            FeatureStrategyConcept* feature =
                FeatureStrategyRegistry::instance().getFeature(id);

            if (!feature) {
                std::cerr << "[ML] WARNING: Feature ID " << id << " not found.\n";
                continue;
            }

            addFeature(feature);
        }

        rebuildModelInfo();
    }

    void saveFeatureIdsToConfig() const
    {
        std::ofstream out(getFeatureConfigPath(), std::ios::out | std::ios::trunc);
        if (!out.is_open()) {
            std::cerr << "ERROR: Cannot save feature config to " << getFeatureConfigPath() << "\n";
            return;
        }

        for (auto* feature : m_featureStrategies) {
            out << feature->getId() << "\n";
        }
    }

};