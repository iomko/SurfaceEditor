#pragma once
#include "FeaturesExtractor.h"
#include "vector"
#include "Structures/ExtendedHalfEdge.h"
#include "FeatureObject.h"
#include "FeatureStrategyIDS.h"

class FeatureStrategyConcept {
public:
    virtual ~FeatureStrategyConcept() = default;
    virtual std::string getName() const = 0;
    virtual FeatureObjectType getObjectType() const = 0;
    virtual int getId() const = 0;
};

template<typename Type>
class FeatureStrategy : public FeatureStrategyConcept {
public:
    FeatureObjectType getObjectType() const override {
        return FeatureObjectTypeTrait<Type>::value;
    }
    virtual std::vector<float> invoke(Type* obj) = 0;
    virtual size_t getSize() = 0;
};