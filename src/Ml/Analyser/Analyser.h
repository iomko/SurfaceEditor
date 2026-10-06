#pragma once
#include "../Analyser/AnalyserIDS.h"
#include <vector>
#include <string>

#include "../FeatureStrategies/FeatureObject.h"
#include "../FeatureStrategies/FeatureStrategy.h"
#include "../../Scene/Mesh.h"

class Analyser {
public:
    virtual ~Analyser() = default;
    virtual std::string getName() const = 0;
};

class Predictor : public Analyser {
public:
    virtual std::vector<float> predict(Mesh* mesh) = 0;
    virtual void train(const std::vector<Mesh*>& meshes) = 0;
    virtual FeatureObjectType getFeatureObjectType() const = 0;
    virtual bool addFeature(FeatureStrategyConcept* feature) = 0;
    virtual bool removeFeature(FeatureStrategyConcept* feature) = 0;
    virtual std::vector<FeatureStrategyConcept*> getFeatures() const = 0;
    virtual std::string getModelConfigPath() const = 0;
};

class Processor : public Analyser {
public:
    virtual void process(Mesh* mesh) = 0;
};