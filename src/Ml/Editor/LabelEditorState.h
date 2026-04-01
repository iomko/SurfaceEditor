#pragma once

#include <string>
#include <vector>

#include "PredictorConfig.h"

struct LabelPropertyOption
{
    std::string displayName;
    int featureId = -1;
    int componentIndex = 0;
};

struct LabelEditorState
{
    std::vector<LabelDefinition> labels;
    std::vector<LabelPropertyOption> propertyOptions;
    int selectedLabelIndex = -1;
};