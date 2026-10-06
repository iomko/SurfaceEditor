#pragma once

#include <string>
#include <fstream>

#include <nlohmann/json.hpp>

#include "PredictorConfig.h"

class PredictorConfigSerializer
{
public:
    static nlohmann::json toJson(const PredictorConfig& config)
    {
        nlohmann::json root;
        root["features"] = nlohmann::json::array();
        root["labels"] = nlohmann::json::array();

        for (int featureId : config.featureIds)
        {
            root["features"].push_back({
                { "id", featureId }
            });
        }

        for (const LabelDefinition& label : config.labels)
        {
            nlohmann::json labelJson;
            labelJson["name"] = label.name;
            labelJson["mode"] = toString(label.mode);
            labelJson["conditions"] = nlohmann::json::array();

            for (const LabelCondition& condition : label.conditions)
            {
                labelJson["conditions"].push_back({
                    { "featureId", condition.featureId },
                    { "componentIndex", condition.componentIndex },
                    { "operator", toString(condition.op) },
                    { "value", condition.value }
                });
            }

            root["labels"].push_back(labelJson);
        }

        return root;
    }

    static bool saveToFile(const std::string& path, const PredictorConfig& config)
    {
        std::ofstream out(path, std::ios::out | std::ios::trunc);
        if (!out.is_open()) {
            return false;
        }

        out << toJson(config).dump(4);
        return true;
    }

private:
    static const char* toString(LabelMatchMode mode)
    {
        switch (mode)
        {
            case LabelMatchMode::All: return "ALL";
            case LabelMatchMode::Any: return "ANY";
            default: return "ALL";
        }
    }

    static const char* toString(LabelOperator op)
    {
        switch (op)
        {
            case LabelOperator::Equal:        return "==";
            case LabelOperator::NotEqual:     return "!=";
            case LabelOperator::Less:         return "<";
            case LabelOperator::LessEqual:    return "<=";
            case LabelOperator::Greater:      return ">";
            case LabelOperator::GreaterEqual: return ">=";
            default: return ">";
        }
    }
};