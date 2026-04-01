#pragma once

#include <string>
#include <fstream>

#include <nlohmann/json.hpp>

#include "PredictorConfig.h"

class PredictorConfigDeserializer
{
public:
    static PredictorConfig fromJson(const nlohmann::json& json)
    {
        PredictorConfig config;

        if (!json.is_object()) {
            return config;
        }

        if (json.contains("features") && json["features"].is_array())
        {
            for (const auto& featureJson : json["features"])
            {
                if (!featureJson.is_object()) {
                    continue;
                }

                if (!featureJson.contains("id") || !featureJson["id"].is_number_integer()) {
                    continue;
                }

                config.featureIds.push_back(featureJson["id"].get<int>());
            }
        }

        if (json.contains("labels") && json["labels"].is_array())
        {
            for (const auto& labelJson : json["labels"])
            {
                if (!labelJson.is_object()) {
                    continue;
                }

                LabelDefinition label;

                if (labelJson.contains("name") && labelJson["name"].is_string()) {
                    label.name = labelJson["name"].get<std::string>();
                }

                if (labelJson.contains("mode") && labelJson["mode"].is_string()) {
                    label.mode = parseMatchMode(labelJson["mode"].get<std::string>());
                }

                if (labelJson.contains("conditions") && labelJson["conditions"].is_array())
                {
                    for (const auto& condJson : labelJson["conditions"])
                    {
                        if (!condJson.is_object()) {
                            continue;
                        }

                        LabelCondition condition;

                        if (condJson.contains("featureId") && condJson["featureId"].is_number_integer()) {
                            condition.featureId = condJson["featureId"].get<int>();
                        }

                        if (condJson.contains("componentIndex") && condJson["componentIndex"].is_number_integer()) {
                            condition.componentIndex = condJson["componentIndex"].get<int>();
                        }

                        if (condJson.contains("operator") && condJson["operator"].is_string()) {
                            condition.op = parseOperator(condJson["operator"].get<std::string>());
                        }

                        if (condJson.contains("value") &&
                            (condJson["value"].is_number_float() || condJson["value"].is_number_integer()))
                        {
                            condition.value = condJson["value"].get<float>();
                        }

                        label.conditions.push_back(condition);
                    }
                }

                config.labels.push_back(label);
            }
        }

        return config;
    }

    static PredictorConfig loadFromFile(const std::string& path)
    {
        std::ifstream in(path);
        if (!in.is_open()) {
            return {};
        }

        try
        {
            nlohmann::json json;
            in >> json;
            return fromJson(json);
        }
        catch (...)
        {
            return {};
        }
    }

private:
    static LabelMatchMode parseMatchMode(const std::string& value)
    {
        if (value == "ANY") {
            return LabelMatchMode::Any;
        }

        return LabelMatchMode::All;
    }

    static LabelOperator parseOperator(const std::string& value)
    {
        if (value == "==") return LabelOperator::Equal;
        if (value == "!=") return LabelOperator::NotEqual;
        if (value == "<")  return LabelOperator::Less;
        if (value == "<=") return LabelOperator::LessEqual;
        if (value == ">")  return LabelOperator::Greater;
        if (value == ">=") return LabelOperator::GreaterEqual;

        return LabelOperator::Greater;
    }
};