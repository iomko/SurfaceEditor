#pragma once
#include <vector>
#include "../../Scene/Mesh.h"
#include "FeatureStrategy.h"

class DataExtractorStrategyConcept {
public:
    virtual std::vector<float> invoke(Mesh* mesh) = 0;
};

template<typename TExtractor>
class DataExtractorStrategy : public DataExtractorStrategyConcept {
public:
    virtual std::vector<float> invoke(Mesh* mesh) override {
        for(FeatureStrategy<TExtractor>* featureStrategy : m_features) {
            (*featureStrategy) 
        }  
    }
private:
    std::vector<FeatureStrategy<TExtractor>*> m_features;
    std::vector<FeatureStrategy<TExtractor>*> m_labels;
};
