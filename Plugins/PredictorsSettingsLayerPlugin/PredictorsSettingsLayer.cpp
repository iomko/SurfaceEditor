#include "../../src/UI/LayerRegistry.h"

#include "../../src/ViewPortsController.h"
#include "../../src/Core/Layer.h"
#include "../../src/Renderer/MaterialRegistry.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/Commands/CommandIDs.h"
#include "../../src/Ml/Analyser/AnalyserRegistry.h"
#include <Styling/Font.h>
#include <Builders/UIButtonBuilder.h>
#include <Builders/UILabelBuilder.h>
#include <Builders/UICheckBoxBuilder.h>
#include <Builders/UIInputBoxBuilder.h>
#include <Builders/UIWindowBuilder.h>
#include <Builders/UICollapsingHeaderBuilder.h>
#include <Builders/UIComboBuilder.h>
#include <Builders/UIChildWindowBuilder.h>
#include <Builders/UITreeBuilder.h>
#include <Builders/UIDragBuilder.h>
#include <Components/Button.h>
#include <Components/CheckBox.h>
#include <Components/ChildWindow.h>
#include <Components/CollapsingHeader.h>
#include <Components/Combo.h>
#include <Components/Component.h>
#include <Components/Drag.h>
#include <Components/InputBox.h>
#include <Components/Label.h>
#include <Components/SameLine.h>
#include <Components/Separator.h>
#include <Components/Tree.h>
#include <Utils/VisibilityHandler.h>

#include "PredictorsSettingsLayer.h"

#include "ImGuiFileDialog.h"

#include "../ImportMeshesPlugin/OBJImporter.h"

#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <utility>

namespace
{
    static constexpr const char *LAYER_NAME = "PREDICTORS_SETTINGS_LAYER";
    static constexpr std::size_t LABEL_EDITOR_LAYOUT_SIZE_ESTIMATE = 21;

    std::shared_ptr<ui::styling::LabelConfig> runtimeTextConfig()
    {
        static auto config = LabelConfigBuilder()
                                 .font(ui::styling::Font::regular(0.02f))
                                 .build();
        return config;
    }

    std::shared_ptr<ui::styling::ButtonConfig> runtimeButtonConfig()
    {
        static auto config = ButtonConfigBuilder()
                                 .rounding(8.0f)
                                 .background(ui::styling::Color::lightGray)
                                 .font(ui::styling::Font::regular(0.6f))
                                 .size(ImVec2{0.18f, 0.04f})
                                 .onHoverColor(ui::styling::Color::hoverOverOrange)
                                 .onClickColor(ui::styling::Color::onClickOrange)
                                 .build();
        return config;
    }

    std::shared_ptr<ui::styling::DragConfig<float>> runtimeDragConfig()
    {
        static auto config = DragConfigBuilder<float>()
                                 .speed(0.01f)
                                 .width(0.18f)
                                 .background(ui::styling::Color::lightGray)
                                 .onHoverBackground(ui::styling::Color::hoverOverOrange)
                                 .onActiveBackground(ui::styling::Color::onClickOrange)
                                 .build();
        return config;
    }

    void renderDisabledText(const std::string& text)
    {
        ui::components::Label<ui::styling::TextTypes::DISABLED> label(text, runtimeTextConfig());
        label.render();
    }

    void renderText(const std::string& text) {
        ui::components::Label<ui::styling::TextTypes::CLASSIC> label(text, runtimeTextConfig());
        label.render();
    }

    class UsedFeaturesList : public ui::components::Component
    {
    public:
        UsedFeaturesList(
            FeatureEditor& featureEditor,
            std::shared_ptr<ui::styling::ButtonConfig> removeButtonConfig)
            : Component("Used Features", removeButtonConfig)
            , m_featureEditor(featureEditor)
            , m_removeButtonConfig(std::move(removeButtonConfig))
        {
        }

        void render() override
        {
            const auto& usedFeatures = m_featureEditor.getState().usedFeatures;
            renderText("Used Features");

            if (usedFeatures.empty())
            {
                renderDisabledText("None");
                return;
            }

            int featureToRemove = -1;
            for (int i = 0; i < static_cast<int>(usedFeatures.size()); ++i)
            {
                ImGui::PushID(i);

                FeatureStrategyConcept* feature = usedFeatures[i];
                if (feature)
                {
                    renderText(feature->getName());
                    ui::components::SameLine sameLine;
                    sameLine.render();

                    ui::components::Button removeButton(
                        "Remove",
                        m_removeButtonConfig,
                        [&featureToRemove, i](ui::components::Button*) {
                            featureToRemove = i;
                        });
                    removeButton.render();
                }
                else
                {
                    renderDisabledText("Unknown feature");
                }

                ImGui::PopID();
            }

            if (featureToRemove >= 0 && featureToRemove < static_cast<int>(usedFeatures.size()))
            {
                m_featureEditor.removeFeature(usedFeatures[featureToRemove]);
            }
        }

    private:
        FeatureEditor& m_featureEditor;
        std::shared_ptr<ui::styling::ButtonConfig> m_removeButtonConfig;
    };

    class SelectedLabelDetails : public ui::components::Component
    {
    public:
        SelectedLabelDetails(
            LabelEditor& labelEditor,
            std::shared_ptr<ui::styling::ComboConfig> comboConfig,
            std::shared_ptr<ui::styling::InputBoxConfig> inputBoxConfig)
            : Component("Selected Label Details", {})
            , m_labelEditor(labelEditor)
            , m_comboConfig(std::move(comboConfig))
            , m_inputBoxConfig(std::move(inputBoxConfig))
        {
        }

        void render() override
        {
            LabelEditorState& labelState = m_labelEditor.getState();
            LabelDefinition* label = m_labelEditor.getSelectedLabel();
            if (label)
            {
                renderText("Label Name");
                ui::components::SameLine nameSameLine;
                nameSameLine.render();
                ui::components::InputBox<char> nameInputBox("Label Name", m_inputBoxConfig);
                std::strncpy(
                    nameInputBox.inputValue(),
                    label->name.c_str(),
                    sizeof(char) * 255);
                nameInputBox.render();
                label->name = nameInputBox.inputValue();

                static const std::vector<std::string> matchModes{"ALL", "ANY"};
                using MatchModeIterator = std::vector<std::string>::const_iterator;
                renderText("Match Mode");
                ui::components::SameLine sameLine;
                sameLine.render();
                ui::components::Combo<MatchModeIterator> matchModeCombo(
                    "MatchMode",
                    m_comboConfig,
                    []() {
                        return std::pair{matchModes.begin(), matchModes.end()};
                    },
                    [&label]() {
                        return matchModes[static_cast<std::size_t>(label->mode)];
                    },
                    [](MatchModeIterator option) {
                        return static_cast<ImGuiID>(std::distance(matchModes.begin(), option) + 1);
                    },
                    [](MatchModeIterator option) {
                        return *option;
                    },
                    [&label](MatchModeIterator option) {
                        return std::distance(matchModes.begin(), option) ==
                               static_cast<std::ptrdiff_t>(label->mode);
                    },
                    [&label](MatchModeIterator option) {
                        label->mode = static_cast<LabelMatchMode>(
                            std::distance(matchModes.begin(), option));
                    });
                matchModeCombo.render();

                ui::components::Separator separator(0.9f);
                separator.render();
                renderText("Conditions");
                int conditionToRemove = -1;

                for (int i = 0; i < static_cast<int>(label->conditions.size()); ++i)
                {
                    ImGui::PushID(i);
                    LabelCondition& condition = label->conditions[i];

                    int selectedPropertyIndex = -1;
                    for (int p = 0; p < static_cast<int>(labelState.propertyOptions.size()); ++p)
                    {
                        if (labelState.propertyOptions[p].featureId == condition.featureId &&
                            labelState.propertyOptions[p].componentIndex == condition.componentIndex)
                        {
                            selectedPropertyIndex = p;
                            break;
                        }
                    }

                    renderText("Property");
                    ui::components::SameLine propertySameLine;
                    propertySameLine.render();
                    using PropertyIterator = std::vector<LabelPropertyOption>::iterator;
                    if (labelState.propertyOptions.empty())
                    {
                        renderDisabledText("None available");
                    }
                    else
                    {
                        ui::components::Combo<PropertyIterator> propertyCombo(
                            "Property",
                            m_comboConfig,
                            [&labelState]() {
                                return std::pair{
                                    labelState.propertyOptions.begin(),
                                    labelState.propertyOptions.end()};
                            },
                            [&labelState, selectedPropertyIndex]() {
                                return selectedPropertyIndex >= 0
                                    ? labelState.propertyOptions[selectedPropertyIndex].displayName
                                    : std::string{"None"};
                            },
                            [&labelState](PropertyIterator option) {
                                return static_cast<ImGuiID>(
                                    std::distance(labelState.propertyOptions.begin(), option) + 1);
                            },
                            [](PropertyIterator option) {
                                return option->displayName;
                            },
                            [&labelState, selectedPropertyIndex](PropertyIterator option) {
                                return selectedPropertyIndex >= 0 &&
                                    std::distance(labelState.propertyOptions.begin(), option) ==
                                        selectedPropertyIndex;
                            },
                            [&condition](PropertyIterator option) {
                                condition.featureId = option->featureId;
                                condition.componentIndex = option->componentIndex;
                            });
                        propertyCombo.render();
                    }

                    static const std::vector<std::string> operators{
                        "==", "!=", "<", "<=", ">", ">="};
                    using OperatorIterator = std::vector<std::string>::const_iterator;
                    renderText("Operator");
                    ui::components::SameLine operatorSameLine;
                    operatorSameLine.render();
                    ui::components::Combo<OperatorIterator> operatorCombo(
                        "Operator",
                        m_comboConfig,
                        []() {
                            return std::pair{operators.begin(), operators.end()};
                        },
                        [&condition]() {
                            return operators[static_cast<std::size_t>(condition.op)];
                        },
                        [](OperatorIterator option) {
                            return static_cast<ImGuiID>(std::distance(operators.begin(), option) + 1);
                        },
                        [](OperatorIterator option) {
                            return *option;
                        },
                        [&condition](OperatorIterator option) {
                            return std::distance(operators.begin(), option) ==
                                   static_cast<std::ptrdiff_t>(condition.op);
                        },
                        [&condition](OperatorIterator option) {
                            condition.op = static_cast<LabelOperator>(
                                std::distance(operators.begin(), option));
                        });
                    operatorCombo.render();
                    renderText("Value");
                    ui::components::SameLine valueSameLine;
                    valueSameLine.render();
                    ui::components::Drag<float> valueDrag("Value", runtimeDragConfig());
                    *valueDrag.inputValue() = condition.value;
                    valueDrag.render();
                    condition.value = *valueDrag.inputValue();

                    ui::components::Button removeConditionButton(
                        "Remove Condition",
                        runtimeButtonConfig(),
                        [&conditionToRemove, i](ui::components::Button*) {
                            conditionToRemove = i;
                        });
                    removeConditionButton.render();
                    ui::components::Separator conditionSeparator(0.9f);
                    conditionSeparator.render();
                    ImGui::PopID();
                }

                if (conditionToRemove >= 0)
                {
                    m_labelEditor.removeConditionFromSelectedLabel(conditionToRemove);
                }

                ui::components::Button addConditionButton(
                    "+ Add Condition",
                    runtimeButtonConfig(),
                    [this](ui::components::Button*) {
                        m_labelEditor.addConditionToSelectedLabel();
                    });
                addConditionButton.render();
            }
            else
            {
                renderText("No label selected.");
            }
        }

    private:
        LabelEditor& m_labelEditor;
        std::shared_ptr<ui::styling::ComboConfig> m_comboConfig;
        std::shared_ptr<ui::styling::InputBoxConfig> m_inputBoxConfig;
    };

    class TrainingSourceSettingsView final : public ui::components::Component
    {
    public:
        TrainingSourceSettingsView(
            std::function<void(ui::components::CheckBox&)> renderSettings,
            std::shared_ptr<ui::styling::CheckBoxConfig> checkBoxConfig,
            bool& trainOnlySelected)
            : Component("Training Source Settings", {})
            , m_renderSettings(std::move(renderSettings))
            , m_trainOnlySelected(trainOnlySelected)
            , m_trainOnlySelectedCheckBox("TrainOnlySelected", std::move(checkBoxConfig))
        {
            *m_trainOnlySelectedCheckBox.isChecked() = m_trainOnlySelected;
        }

        void render() override
        {
            m_renderSettings(m_trainOnlySelectedCheckBox);
            m_trainOnlySelected = *m_trainOnlySelectedCheckBox.isChecked();
        }

    private:
        std::function<void(ui::components::CheckBox&)> m_renderSettings;
        bool& m_trainOnlySelected;
        ui::components::CheckBox m_trainOnlySelectedCheckBox;
    };

    std::shared_ptr<ui::styling::ChildWindowConfig> makeLabelChildConfig(
        const std::string& name,
        float width,
        float height)
    {
        return ChildWindowConfigBuilder()
            .name(name)
            .background(ui::styling::Color::transparentGray)
            .rounding(8.0f)
            .borderSize(1.0f)
            .size(ChildWindowSizeConfigBuilder()
                      .width(width)
                      .height(height)
                      .minWidth(0.0f)
                      .minHeight(0.0f)
                      .build())
            .build();
    }
}

static AutoRegisterLayerArgs<PredictorsSettingsLayer, std::string> reg("PREDICTORS_SETTINGS_LAYER");

PredictorsSettingsLayer::PredictorsSettingsLayer(const std::string& name)
    : Layer(name), ui::components::Window(LAYER_NAME)
{
    VisibilityHandler::show(LAYER_NAME);
}

void PredictorsSettingsLayer::onEvent(Event& event)
{
    if (event.getType() == EventType::MouseButtonPress)
    {
        if (m_isMouseInsideWindow)
        {
            event.isHandled = true;
        }
    }
}

void PredictorsSettingsLayer::initWindowConfig()
{
    auto posConfig = WindowPosConfigBuilder()
                         .posX(0.0f)
                         .posY(0.0f)
                         .build();

    auto sizeConfig = WindowSizeConfigBuilder()
                          .width(0.48f)
                          .height(0.72f)
                          .minWidth(0.01f)
                          .minHeight(0.01f)
                          .build();

    auto titleBarConfig = WindowTitleBarBuilder()
                              .background(ui::styling::Color::titleBarOrange)
                              .font(ui::styling::Font::extraBold(20.0f))
                              .build();

    m_windowConfig = WindowConfigBuilder()
                         .name("Predictors Settings Layer")
                         .layerName(LAYER_NAME)
                         .background(ui::styling::Color::transparentGray)
                         .titleBar(titleBarConfig)
                         .pos(posConfig)
                         .size(sizeConfig)
                         .build();
}

void PredictorsSettingsLayer::initComponents()
{
    using namespace ui::components;

    auto buttonConfig = ButtonConfigBuilder()
                            .rounding(8.0f)
                            .background(ui::styling::Color::lightGray)
                            .font(ui::styling::Font::regular(0.6f))
                            .size(ImVec2{0.15f, 0.04f})
                            .onHoverColor(ui::styling::Color::hoverOverOrange)
                            .onClickColor(ui::styling::Color::onClickOrange)
                            .build();

    auto featureRemoveButtonConfig = ButtonConfigBuilder()
                                         .rounding(8.0f)
                                         .background(ui::styling::Color::lightGray)
                                         .font(ui::styling::Font::regular(0.6f))
                                         .size(ImVec2{0.08f, 0.04f})
                                         .onHoverColor(ui::styling::Color::hoverOverOrange)
                                         .onClickColor(ui::styling::Color::onClickOrange)
                                         .build();

    auto textConfig = LabelConfigBuilder()
                          .font(ui::styling::Font::regular(0.02f))
                          .build();

    auto headingConfig = LabelConfigBuilder()
                             .font(ui::styling::Font::extraBold(0.025f))
                             .build();

    auto collapsingHeaderConfig = CollapsingHeaderConfigBuilder()
                                      .header(ui::styling::Color::titleBarOrange)
                                      .headerHovered(ui::styling::Color::titleBarOrange)
                                      .headerActive(ui::styling::Color::titleBarOrange)
                                      .framePadding(ImVec2{6.0f, 4.0f})
                                      .flags(ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth)
                                      .build();

    auto checkBoxConfig = CheckBoxConfigBuilder()
                              .checkMarkColor(ui::styling::Color::selectedOrange)
                              .background(ui::styling::Color::lightGray)
                              .onHoverBackground(ui::styling::Color::lightGray)
                              .onActiveBackground(ui::styling::Color::lightGray)
                              .build();

    auto inputBoxConfig = InputBoxBuilder()
                              .width(0.18f)
                              .background(ui::styling::Color::lightGray)
                              .build();

    auto comboConfig = ComboConfigBuilder()
                           .frameBackground(ui::styling::Color::lightGray)
                           .frameBackgroundHovered(ui::styling::Color::hoverOverOrange)
                           .frameBackgroundActive(ui::styling::Color::onClickOrange)
                           .border(ui::styling::Color::darkGray)
                           .text(ImVec4{1.0f, 1.0f, 1.0f, 1.0f})
                           .popUpBackground(ui::styling::Color::gray)
                           .header(ui::styling::Color::selectedOrange)
                           .headerHovered(ui::styling::Color::hoverOverOrange)
                           .headerActive(ui::styling::Color::onClickOrange)
                           .scrollBarBackground(ui::styling::Color::darkGray)
                           .scrollBarGrab(ui::styling::Color::lightGray)
                           .scrollBarGrabHovered(ui::styling::Color::hoverOverOrange)
                           .scrollBarGrabActive(ui::styling::Color::onClickOrange)
                           .navCursor(ui::styling::Color::titleBarOrange)
                           .framePadding(ImVec2{6.0f, 4.0f})
                           .frameRounding(8.0f)
                           .frameBorderSize(0.0f)
                           .popUpRounding(8.0f)
                           .popUpBorderSize(1.0f)
                           .build();

    Layout* layout = emplaceLayout();
    layout->reserveComponents(40);

    emplaceComponent<Label<ui::styling::TextTypes::CLASSIC>>(layout, "Choose Predictor:", headingConfig);
    using OptionIterator = std::vector<AnalyserRegistry::InstanceInfo>::iterator;
    emplaceComponent<ui::components::Combo<OptionIterator>>(layout, "Select Predictor", comboConfig,
                            [this](){
                                return std::pair{m_predictorInstances.begin(), m_predictorInstances.end()};
                            },
                            [this](){
                                if (m_selectedPredictorIndex >= 0 &&
                                    m_selectedPredictorIndex < static_cast<int>(m_predictorInstances.size())) {
                                    return m_predictorInstances[m_selectedPredictorIndex].displayName;
                                }
                                return std::string{"None"};
                            },
                            [this](OptionIterator option){
                                return ImGui::GetID(option->instanceId.c_str());
                            },
                            [this](OptionIterator option) {
                                return option->displayName;
                            },
                            [this](OptionIterator option) {
                                return m_selectedPredictorIndex >= 0 &&
                                       m_selectedPredictorIndex < static_cast<int>(m_predictorInstances.size()) &&
                                       option->instanceId == m_predictorInstances[m_selectedPredictorIndex].instanceId;
                            },
                            [this](OptionIterator option) {
                                m_selectedPredictorIndex = static_cast<int>(option - m_predictorInstances.begin());
                                m_predictorEditor.open(getSelectedPredictor());

                                Predictor* predictor = getSelectedPredictor();
                                std::memset(m_predictorNameBuffer, 0, sizeof(m_predictorNameBuffer));
                                if (predictor) {
                                    std::strncpy(m_predictorNameBuffer, predictor->getName().c_str(),
                                                 sizeof(m_predictorNameBuffer) - 1);
                                }
                                if (m_predictorNameInputBox)
                                {
                                    std::strncpy(
                                        m_predictorNameInputBox->inputValue(),
                                        m_predictorNameBuffer,
                                        sizeof(m_predictorNameBuffer) - 1);
                                }
                            });
    m_predictorContent = emplaceComponent<ChildWindow>(
        layout, "PredictorContent", makeLabelChildConfig("PredictorContent", 0.48f, 0.72f));
    Layout* predictorContentLayout = m_predictorContent->emplaceLayout();
    predictorContentLayout->reserveComponents(30);

    m_predictorContent->emplaceComponent<Separator>(predictorContentLayout, 0.9f);

    auto featureEditorHeader = m_predictorContent->emplaceComponent<CollapsingHeader>(
        predictorContentLayout, "Feature Editor", collapsingHeaderConfig);
    Layout* featureEditorLayout = featureEditorHeader->emplaceLayout();
    featureEditorLayout->reserveComponents(1);
    featureEditorHeader->emplaceComponent<UsedFeaturesList>(
        featureEditorLayout, m_predictorEditor.getFeatureEditor(), featureRemoveButtonConfig);

    m_predictorContent->emplaceComponent<Separator>(predictorContentLayout, 0.9f);

    auto labelEditorHeader = m_predictorContent->emplaceComponent<CollapsingHeader>(
        predictorContentLayout, "Label Editor", collapsingHeaderConfig);
    Layout* labelEditorLayout = labelEditorHeader->emplaceLayout();
    labelEditorLayout->reserveComponents(4);
    labelEditorHeader->emplaceComponent<Label<ui::styling::TextTypes::CLASSIC>>(
        labelEditorLayout, "Label Editor", textConfig);

    auto labelColumn = labelEditorHeader->emplaceComponent<ChildWindow>(
        labelEditorLayout, "LabelEditorColumn", makeLabelChildConfig("LabelEditorColumn", 0.16f, 0.32f));
    Layout* labelColumnLayout = labelColumn->emplaceLayout();
    labelColumnLayout->reserveComponents(2);

    auto labelTreePanel = labelColumn->emplaceComponent<ChildWindow>(
        labelColumnLayout, "LabelsTreePanel", makeLabelChildConfig("LabelsTreePanel", 0.16f, 0.26f));
    Layout* labelTreeLayout = labelTreePanel->emplaceLayout();
    labelTreeLayout->reserveComponents(1);

    using LabelIterator = std::vector<LabelDefinition>::iterator;
    auto treeConfig = TreeConfigBuilder()
                          .header(ui::styling::Color::selectedOrange)
                          .headerHovered(ui::styling::Color::hoverOverOrange)
                          .headerActive(ui::styling::Color::onClickOrange)
                          .framePadding(ImVec2{6.0f, 4.0f})
                          .flags(ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DrawLinesFull)
                          .build();
    labelTreePanel->emplaceComponent<Tree<LabelDefinition, LabelIterator>>(
        labelTreeLayout,
        "LabelTree",
        treeConfig,
        [this]() {
            auto& labels = m_predictorEditor.getLabelEditor().getState().labels;
            return std::pair{labels.begin(), labels.end()};
        },
        [](LabelIterator label) {
            return std::pair{label, label};
        },
        [this](LabelIterator label) {
            auto& labels = m_predictorEditor.getLabelEditor().getState().labels;
            return static_cast<ImGuiID>(std::distance(labels.begin(), label) + 1);
        },
        [](LabelIterator label) {
            return label->name;
        },
        [this](LabelIterator label) {
            auto& state = m_predictorEditor.getLabelEditor().getState();
            state.selectedLabelIndex = static_cast<int>(
                std::distance(state.labels.begin(), label));
        });

    auto labelActionsPanel = labelColumn->emplaceComponent<ChildWindow>(
        labelColumnLayout, "LabelActionsPanel", makeLabelChildConfig("LabelActionsPanel", 0.16f, 0.06f));
    Layout* labelActionsLayout = labelActionsPanel->emplaceLayout();
    labelActionsLayout->reserveComponents(3);
    labelActionsPanel->emplaceComponent<Button>(
        labelActionsLayout, "+ Add Label", featureRemoveButtonConfig,
        [this](Button*) { m_predictorEditor.getLabelEditor().addLabel(); });
    labelActionsPanel->emplaceComponent<SameLine>(labelActionsLayout);
    labelActionsPanel->emplaceComponent<Button>(
        labelActionsLayout, "Delete Label", featureRemoveButtonConfig,
        [this](Button*) { m_predictorEditor.getLabelEditor().removeSelectedLabel(); });

    labelEditorHeader->emplaceComponent<SameLine>(labelEditorLayout);
    auto labelDetailsPanel = labelEditorHeader->emplaceComponent<ChildWindow>(
        labelEditorLayout, "SelectedLabelEditor", makeLabelChildConfig("SelectedLabelEditor", 0.29f, 0.32f));
    Layout* labelDetailsLayout = labelDetailsPanel->emplaceLayout();
    labelDetailsLayout->reserveComponents(1);
    labelDetailsPanel->emplaceComponent<SelectedLabelDetails>(
        labelDetailsLayout, m_predictorEditor.getLabelEditor(), comboConfig, inputBoxConfig);

    m_predictorContent->emplaceComponent<Label<ui::styling::TextTypes::CLASSIC>>(
        predictorContentLayout, "Predictor Name", textConfig);
    m_predictorNameInputBox = m_predictorContent->emplaceComponent<InputBox<char>>(
        predictorContentLayout, "PredictorName", inputBoxConfig);
    m_predictorContent->emplaceComponent<Separator>(predictorContentLayout, 0.9f);

    m_predictorContent->emplaceComponent<Label<ui::styling::TextTypes::CLASSIC>>(
        predictorContentLayout, "Training Mesh Source", headingConfig);
    m_predictorContent->emplaceComponent<Label<ui::styling::TextTypes::CLASSIC>>(
        predictorContentLayout, "Source", textConfig);
    using TrainingSourceIterator = std::vector<std::string>::iterator;
    m_predictorContent->emplaceComponent<ui::components::Combo<TrainingSourceIterator>>(
        predictorContentLayout,
        "TrainingSource",
        comboConfig,
        [this]() {
            return std::pair{m_trainingSourceOptions.begin(), m_trainingSourceOptions.end()};
        },
        [this]() {
            return m_trainingSourceOptions[static_cast<std::size_t>(m_trainingMeshSourceMode)];
        },
        [this](TrainingSourceIterator option) {
            return static_cast<ImGuiID>(std::distance(m_trainingSourceOptions.begin(), option) + 1);
        },
        [](TrainingSourceIterator option) {
            return *option;
        },
        [this](TrainingSourceIterator option) {
            return std::distance(m_trainingSourceOptions.begin(), option) ==
                   static_cast<std::ptrdiff_t>(m_trainingMeshSourceMode);
        },
        [this](TrainingSourceIterator option) {
            m_trainingMeshSourceMode = static_cast<TrainingMeshSourceMode>(
                std::distance(m_trainingSourceOptions.begin(), option));
        });
    m_predictorContent->emplaceComponent<TrainingSourceSettingsView>(
        predictorContentLayout,
        [this](CheckBox& checkBox) { renderTrainingSourceSettings(checkBox); },
        checkBoxConfig,
        m_trainOnlySelected);
    m_predictorContent->emplaceComponent<Button>(
        predictorContentLayout, "Retrain Current", buttonConfig, [](Button*) {});
    m_predictorContent->emplaceComponent<SameLine>(predictorContentLayout);
    m_predictorContent->emplaceComponent<Button>(
        predictorContentLayout, "Train As New Copy", buttonConfig, [](Button*) {});

    const std::size_t componentCount = layout->components().size()
                                     + predictorContentLayout->components().size()
                                     + featureEditorLayout->components().size()
                                     + LABEL_EDITOR_LAYOUT_SIZE_ESTIMATE;
    m_windowConfig.size.height = static_cast<float>(componentCount) * 0.018f;
}

void PredictorsSettingsLayer::refreshPredictorInstances()
{
    m_predictorInstances = AnalyserRegistry::instance().getPredictorInstances();
}

Predictor* PredictorsSettingsLayer::getSelectedPredictor() const
{
    if (m_selectedPredictorIndex < 0 ||
        m_selectedPredictorIndex >= static_cast<int>(m_predictorInstances.size()))
    {
        return nullptr;
    }

    Analyser* analyser =
        AnalyserRegistry::instance().getAnalyserInstance(m_predictorInstances[m_selectedPredictorIndex].instanceId);

    if (!analyser) {
        return nullptr;
    }

    return dynamic_cast<Predictor*>(analyser);
}

void PredictorsSettingsLayer::renderTrainingSourceSettings(
    ui::components::CheckBox& trainOnlySelectedCheckBox)
{
    if (m_trainingMeshSourceMode == TrainingMeshSourceMode::SceneMeshes)
    {
        trainOnlySelectedCheckBox.render();
        ui::components::SameLine sameLine;
        sameLine.render();
        renderText("Train only on selected");

        if (m_trainOnlySelected)
        {
            renderDisabledText("Training will use only selected meshes.");
        }
        else
        {
            renderDisabledText("Training will use all meshes in the scene.");
        }
    }
    else if (m_trainingMeshSourceMode == TrainingMeshSourceMode::ObjFiles)
    {
        bool selectObjFiles = false;
        ui::components::Button selectObjFilesButton(
            "Select OBJ Files",
            runtimeButtonConfig(),
            [&selectObjFiles](ui::components::Button*) {
                selectObjFiles = true;
            });
        selectObjFilesButton.render();

        if (selectObjFiles)
        {
            IGFD::FileDialogConfig config;
            config.path = ".";
            config.countSelectionMax = 0;
            config.flags = 0;

            ImGuiFileDialog::Instance()->OpenDialog(
                "ChooseObjFilesDialog",
                "Choose OBJ Files",
                ".obj",
                config
            );
        }

        if (ImGuiFileDialog::Instance()->Display("ChooseObjFilesDialog"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                m_selectedObjFilePaths.clear();

                const auto selection = ImGuiFileDialog::Instance()->GetSelection();

                for (const auto& selectedFile : selection)
                {
                    m_selectedObjFilePaths.push_back(selectedFile.second);
                }
            }

            ImGuiFileDialog::Instance()->Close();
        }

        renderText("Selected OBJ files: " + std::to_string(m_selectedObjFilePaths.size()));

        auto selectedFilesConfig = ChildWindowConfigBuilder()
                                       .name("SelectedObjFilesList")
                                       .background(ui::styling::Color::transparentGray)
                                       .rounding(8.0f)
                                       .borderSize(1.0f)
                                       .size(ChildWindowSizeConfigBuilder()
                                                 .width(0.46f)
                                                 .height(0.09f)
                                                 .minWidth(0.0f)
                                                 .minHeight(0.0f)
                                                 .build())
                                       .build();
        ui::components::ChildWindow selectedFilesWindow(
            "SelectedObjFilesList",
            selectedFilesConfig);
        ui::components::Layout* selectedFilesLayout = selectedFilesWindow.emplaceLayout();
        selectedFilesLayout->reserveComponents(static_cast<int>(
            std::max<std::size_t>(m_selectedObjFilePaths.size(), 1)));
        if (m_selectedObjFilePaths.empty())
        {
            selectedFilesWindow.emplaceComponent<
                ui::components::Label<ui::styling::TextTypes::DISABLED>>(
                selectedFilesLayout, "No OBJ files selected.", runtimeTextConfig());
        }
        else
        {
            for (const std::string& filePath : m_selectedObjFilePaths)
            {
                selectedFilesWindow.emplaceComponent<
                    ui::components::Label<ui::styling::TextTypes::BULLET>>(
                    selectedFilesLayout, filePath, runtimeTextConfig());
            }
        }
        selectedFilesWindow.render();

        if (!m_selectedObjFilePaths.empty())
        {
            bool clearObjSelection = false;
            ui::components::Button clearObjSelectionButton(
                "Clear OBJ Selection",
                runtimeButtonConfig(),
                [&clearObjSelection](ui::components::Button*) {
                    clearObjSelection = true;
                });
            clearObjSelectionButton.render();

            if (clearObjSelection)
            {
                m_selectedObjFilePaths.clear();
            }
        }
    }
}

std::vector<Mesh*> PredictorsSettingsLayer::collectSceneTrainingMeshes()
{
    std::vector<Mesh*> meshes;

    if (m_trainOnlySelected)
    {
        SelectionController* selectionController = ViewPortsHolderContext::s_selectionController.get();

        if (!selectionController)
        {
            std::cerr << "[ML] ERROR: SelectionController is null." << std::endl;
            return meshes;
        }

        const SelectionHolder& selectionHolder = selectionController->getHolder();
        meshes = selectionHolder.meshes;
    }
    else
    {
        SceneResources::MeshFaceOctreeCoordsMap meshFacesPairMap = ViewPortsHolderContext::s_viewPortsController->m_scene->m_res.meshFaceOctreeCoordsMap;

        for (auto& [mesh, _] : meshFacesPairMap) {
            meshes.emplace_back(mesh);
        }
    }

    return meshes;
}

std::vector<Mesh*> PredictorsSettingsLayer::collectObjTrainingMeshes()
{
    std::vector<Mesh*> meshes;

    if (m_selectedObjFilePaths.empty())
    {
        std::cerr << "[ML] ERROR: No OBJ files selected." << std::endl;
        return meshes;
    }

    for (const std::string& filePath : m_selectedObjFilePaths) {

        OBJImporter objImporter;
        objImporter.setFilePath(filePath);
        objImporter.read();

        for (Mesh* mesh : objImporter.getMeshes()) {
            
            if(mesh) {
                meshes.emplace_back(mesh);
            }

        }
    }

    return meshes;
}

std::vector<Mesh*> PredictorsSettingsLayer::collectTrainingMeshes()
{
    if (m_trainingMeshSourceMode == TrainingMeshSourceMode::SceneMeshes)
    {
        return collectSceneTrainingMeshes();
    }

    if (m_trainingMeshSourceMode == TrainingMeshSourceMode::ObjFiles)
    {
        return collectObjTrainingMeshes();
    }

    return {};
}

void PredictorsSettingsLayer::onImGuiRender()
{
    bool initializeNameInput = false;
    if (!m_initialized)
    {
        initializeNameInput = true;
        refreshPredictorInstances();

        if (!m_predictorInstances.empty())
        {
            m_selectedPredictorIndex = 0;
            m_predictorEditor.open(getSelectedPredictor());

            Predictor* predictor = getSelectedPredictor();
            if (predictor)
            {
                std::strncpy(m_predictorNameBuffer, predictor->getName().c_str(), sizeof(m_predictorNameBuffer) - 1);
            }
        }

        m_previousPredictorIndex = m_selectedPredictorIndex;
        m_initialized = true;
    }

    if (m_predictorContent)
    {
        m_predictorContent->setIsVisible(m_predictorEditor.getPredictor() != nullptr);
    }

    const bool hasNameInputBeforeRender = m_predictorNameInputBox != nullptr;
    if (initializeNameInput && hasNameInputBeforeRender)
    {
        std::strncpy(
            m_predictorNameInputBox->inputValue(),
            m_predictorNameBuffer,
            sizeof(m_predictorNameBuffer) - 1);
    }

    this->render();
    if (m_predictorNameInputBox)
    {
        if (initializeNameInput && !hasNameInputBeforeRender)
        {
            std::strncpy(
                m_predictorNameInputBox->inputValue(),
                m_predictorNameBuffer,
                sizeof(m_predictorNameBuffer) - 1);
        }
        else
        {
            std::strncpy(
                m_predictorNameBuffer,
                m_predictorNameInputBox->inputValue(),
                sizeof(m_predictorNameBuffer) - 1);
        }
    }
}