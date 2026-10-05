#pragma once

#include <vector>

#include "../FeatureStrategies/FeatureStrategy.h"

struct FeatureEditorState
{
    std::vector<FeatureStrategyConcept*> usedFeatures;
    std::vector<FeatureStrategyConcept*> notUsedFeatures;
    std::vector<FeatureStrategyConcept*> toBeAddedFeatures;
    std::vector<FeatureStrategyConcept*> toBeRemovedFeatures;

    int selectedFeatureToAddIndex = 0;
};