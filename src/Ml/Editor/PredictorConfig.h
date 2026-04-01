#pragma once

#include <string>
#include <vector>

enum class LabelMatchMode
{
    All,
    Any
};

enum class LabelOperator
{
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual
};

struct LabelCondition
{
    int featureId = -1;
    int componentIndex = 0;
    LabelOperator op = LabelOperator::Greater;
    float value = 0.0f;
};

struct LabelDefinition
{
    std::string name;
    LabelMatchMode mode = LabelMatchMode::All;
    std::vector<LabelCondition> conditions;
};

struct PredictorConfig
{
    std::vector<int> featureIds;
    std::vector<LabelDefinition> labels;
};