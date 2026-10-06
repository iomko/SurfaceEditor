#pragma once

#include <vector>
#include <string>

#include "../Analyser/Analyser.h"
#include "../FeatureStrategies/FeatureStrategyRegistry.h"

#include "PredictorConfig.h"
#include "LabelEditorState.h"

class LabelEditor
{
public:
    void initialize(Predictor* predictor, const PredictorConfig& config)
    {
        m_state = {};
        m_state.labels = config.labels;

        buildPropertyOptions(predictor);

        if (!m_state.labels.empty()) {
            m_state.selectedLabelIndex = 0;
        }
    }

    LabelEditorState& getState()
    {
        return m_state;
    }

    const LabelEditorState& getState() const
    {
        return m_state;
    }

    void addLabel()
    {
        LabelDefinition label;
        label.name = "Label" + std::to_string(m_state.labels.size());
        label.mode = LabelMatchMode::All;

        m_state.labels.push_back(label);
        m_state.selectedLabelIndex = static_cast<int>(m_state.labels.size()) - 1;
    }

    void removeSelectedLabel()
    {
        if (m_state.selectedLabelIndex < 0 ||
            m_state.selectedLabelIndex >= static_cast<int>(m_state.labels.size()))
        {
            return;
        }

        m_state.labels.erase(m_state.labels.begin() + m_state.selectedLabelIndex);

        if (m_state.selectedLabelIndex >= static_cast<int>(m_state.labels.size())) {
            m_state.selectedLabelIndex = m_state.labels.empty()
                ? -1
                : static_cast<int>(m_state.labels.size()) - 1;
        }
    }

    void addConditionToSelectedLabel()
    {
        LabelDefinition* label = getSelectedLabel();
        if (!label) {
            return;
        }

        LabelCondition condition;
        if (!m_state.propertyOptions.empty())
        {
            condition.featureId = m_state.propertyOptions[0].featureId;
            condition.componentIndex = m_state.propertyOptions[0].componentIndex;
        }

        label->conditions.push_back(condition);
    }

    void removeConditionFromSelectedLabel(int conditionIndex)
    {
        LabelDefinition* label = getSelectedLabel();
        if (!label) {
            return;
        }

        if (conditionIndex < 0 ||
            conditionIndex >= static_cast<int>(label->conditions.size()))
        {
            return;
        }

        label->conditions.erase(label->conditions.begin() + conditionIndex);
    }

    LabelDefinition* getSelectedLabel()
    {
        if (m_state.selectedLabelIndex < 0 ||
            m_state.selectedLabelIndex >= static_cast<int>(m_state.labels.size()))
        {
            return nullptr;
        }

        return &m_state.labels[m_state.selectedLabelIndex];
    }

    const LabelDefinition* getSelectedLabel() const
    {
        if (m_state.selectedLabelIndex < 0 ||
            m_state.selectedLabelIndex >= static_cast<int>(m_state.labels.size()))
        {
            return nullptr;
        }

        return &m_state.labels[m_state.selectedLabelIndex];
    }

    std::vector<LabelDefinition> collectLabels() const
    {
        return m_state.labels;
    }

private:
    void buildPropertyOptions(Predictor* predictor)
    {
        m_state.propertyOptions.clear();

        if (!predictor) {
            return;
        }

        std::vector<FeatureStrategyConcept*> allFeatures =
            FeatureStrategyRegistry::instance().getFeaturesByType(predictor->getFeatureObjectType());

        for (FeatureStrategyConcept* feature : allFeatures)
        {
            if (!feature) {
                continue;
            }

            const size_t size = feature->getSize();

            if (size <= 1)
            {
                LabelPropertyOption option;
                option.displayName = feature->getName();
                option.featureId = feature->getId();
                option.componentIndex = 0;
                m_state.propertyOptions.push_back(option);
            }
            else
            {
                for (size_t component = 0; component < size; ++component)
                {
                    LabelPropertyOption option;
                    option.displayName =
                        feature->getName() + "[" + std::to_string(component) + "]";
                    option.featureId = feature->getId();
                    option.componentIndex = static_cast<int>(component);
                    m_state.propertyOptions.push_back(option);
                }
            }
        }
    }

private:
    LabelEditorState m_state;
};