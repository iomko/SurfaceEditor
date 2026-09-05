#include "AdditionLayer.h"
#include "imgui.h"
#include "../src/UI/Components/Label.h"
#include "../src/UI/Components/CheckBox.h"
#include "../src/UI/Components/InputBox.h"
#include "../src/UI/Components/Slider.h"
#include "../src/UI/Components/SameLine.h"
#include "../src/UI/Components/Separator.h"
#include "../src/UI/Components/Dummy.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/UI/Components/Button.h"
#include "../../src/UI/Styling/Font.h"
#include "../../src/UI/WindowLayerBus.h"
#include "../../src/UI/Events.h"
#include "../../src/Builders/UIWindowBuilder.h"
#include "../../src/Builders/UIButtonBuilder.h"
#include "../../src/Builders/UILabelBuilder.h"
#include "../../src/Builders/UICheckBoxBuilder.h"
#include "../../src/Builders/UIInputBoxBuilder.h"
#include "../../src/Builders/UISliderBuilder.h"

namespace
{
    static constexpr const char* LAYER_NAME = "ADDITION_LAYER";
} // namespace

static AutoRegisterLayerArgs<AdditionLayer, std::string> reg("ADDITION_LAYER");

AdditionLayer::AdditionLayer(const std::string& name)
    : Layer(name)
    , ui::IWindow(LAYER_NAME)
    , m_additionType(AdditionType::NONE)
{
    initWindowConfig();
    initComponents();
    initConnections();
}

void AdditionLayer::initConnections()
{
    WindowLayerBus::on<AdditionLayerState>([this](AdditionLayerState& state) {
        m_additionType = static_cast<AdditionType>(state.additionType);
    });
}

void AdditionLayer::initWindowConfig()
{
    auto posConfig = WindowPosConfigBuilder()
        .posX(0.38f)
        .posY(0.28f)
        .build();

    auto sizeConfig = WindowSizeConfigBuilder()
        .width(0.25f)
        .height(0.4f)
        .minWidth(0.6f)
        .minHeight(0.75f)
        .build();

    auto titleBarConfig = WindowTitleBarBuilder()
        .background(ui::styling::Color::titleBarOrange)
        .font(ui::styling::Font::extraBold(20.0f))
        .build();

    auto flags = ImGuiWindowFlags_NoResize
               | ImGuiWindowFlags_NoMove
               | ImGuiWindowFlags_NoScrollbar
               | ImGuiWindowFlags_NoCollapse;
    
    m_windowConfig = WindowConfigBuilder()
        .name("\tEnter parameters")
        .layerName(LAYER_NAME)
        .background(ui::styling::Color::transparentGray)
        .rounding(20.0f)
        .pos(posConfig)
        .size(sizeConfig)
        .titleBar(titleBarConfig)
        .flags(flags)
        .build();
}

void AdditionLayer::initComponents()
{
    initMeshComponents();
    initSurfaceComponents();
    initDialogButtons();
}

void AdditionLayer::initDialogButtons()
{
    using namespace ui::components;

    static constexpr int dialogItemsCount = 3;

    auto dialogButtonConfig = ButtonConfigBuilder()
        .rounding(8.0f)
        .background(ui::styling::Color::lightGray)
        .font(ui::styling::Font::regular(0.6f))
        .size(ImVec2{ 0.042f, 0.04f })
        .onHoverColor(ui::styling::Color::hoverOverOrange)
        .onClickColor(ui::styling::Color::onClickOrange)
        .build();

    auto dummyCallback = [](Button*){};

    ui::Layout* m_dialogLayout = emplaceLayout(ImVec2{ 0.28f, 0.0f });
    m_dialogLayout->reserveComponents(dialogItemsCount);

    m_createButton = emplaceComponent<Button>(m_dialogLayout, "Add", dialogButtonConfig, dummyCallback);
    emplaceComponent<SameLine>(m_dialogLayout);
    emplaceComponent<Button>(m_dialogLayout, "Cancel", dialogButtonConfig, [](Button*) {
        VisibilityHandler::hide("ADDITION_LAYER");
    });

    m_subdivisionSlider->linkWith(m_subdivisionInputBox);
    m_sizeSlider->linkWith(m_sizeInputBox);

    *m_subdivisionInputBox->inputValue() = 1;
    *m_sizeInputBox->inputValue()        = 1.0f;
}

void AdditionLayer::initMeshComponents()
{
    using namespace ui::components;

    static constexpr int mainItemsCount = 31;

    auto headlinersConfig = LabelConfigBuilder()
        .font(ui::styling::Font::extraBold(0.025f))
        .build();

    auto defaultTextConfig = LabelConfigBuilder()
        .font(ui::styling::Font::regular(0.02))
        .build();

    auto checkBoxConfig = CheckBoxConfigBuilder()
        .checkMarkColor(ui::styling::Color::selectedOrange)
        .background(ui::styling::Color::lightGray)
        .onHoverBackground(ui::styling::Color::lightGray)
        .onActiveBackground(ui::styling::Color::lightGray)
        .build();

    auto inputBoxConfig = InputBoxBuilder()
        .width(0.17f)
        .background(ui::styling::Color::lightGray)
        .build();

    auto intSliderConfig = SliderConfigBuilder<int>()
        .width(0.37f)
        .max(100)
        .background(ui::styling::Color::lightGray)
        .onHoverBackground(ui::styling::Color::lightGray)
        .onActiveBackground(ui::styling::Color::lightGray)
        .grabBackground(ui::styling::Color::titleBarOrange)
        .onGrabActiveBackground(ui::styling::Color::titleBarOrange)
        .build();

    auto floatSliderConfig = SliderConfigBuilder<float>()
        .width(0.36f)
        .max(100.0f)
        .background(ui::styling::Color::lightGray)
        .onHoverBackground(ui::styling::Color::lightGray)
        .onActiveBackground(ui::styling::Color::lightGray)
        .grabBackground(ui::styling::Color::titleBarOrange)
        .onGrabActiveBackground(ui::styling::Color::titleBarOrange)
        .build();

    m_meshLayout = emplaceLayout(ImVec2{ 0.2f, 0.05f });
    m_meshLayout->reserveComponents(mainItemsCount);

    emplaceComponent<Label>(m_meshLayout, "Subdivision", headlinersConfig);
    emplaceComponent<Label>(m_meshLayout, "Automatic:", defaultTextConfig);
    emplaceComponent<SameLine>(m_meshLayout);
    m_automaticCheckBox   = emplaceComponent<CheckBox>(m_meshLayout, "AutomaticCheckBox", checkBoxConfig);
    m_subdivisionInputBox = emplaceComponent<InputBox<int>>(m_meshLayout, "SubdivisionInputBox", inputBoxConfig);
    emplaceComponent<SameLine>(m_meshLayout);
    m_subdivisionSlider = emplaceComponent<Slider<int>>(m_meshLayout, "SubdivisionSlider", intSliderConfig);
    emplaceComponent<Separator>(m_meshLayout, 0.55f);
    emplaceComponent<Label>(m_meshLayout, "Size", headlinersConfig);
    m_sizeInputBox = emplaceComponent<InputBox<float>>(m_meshLayout, "SizeInputBox", inputBoxConfig);
    emplaceComponent<SameLine>(m_meshLayout);
    m_sizeSlider = emplaceComponent<Slider<float>>(m_meshLayout, "SizeSlider", floatSliderConfig);
    emplaceComponent<Separator>(m_meshLayout, 0.55f);
    emplaceComponent<Label>(m_meshLayout, "Position", headlinersConfig);
    emplaceComponent<Dummy>(m_meshLayout, 0.003f, 0.0f);
    emplaceComponent<SameLine>(m_meshLayout);
    emplaceComponent<Label>(m_meshLayout, "X:", defaultTextConfig);
    emplaceComponent<SameLine>(m_meshLayout);
    emplaceComponent<Dummy>(m_meshLayout, 0.13f, 0.0f);
    emplaceComponent<SameLine>(m_meshLayout);
    emplaceComponent<Label>(m_meshLayout, "Y:", defaultTextConfig);
    emplaceComponent<SameLine>(m_meshLayout);
    emplaceComponent<Dummy>(m_meshLayout, 0.13f, 0.0f);
    emplaceComponent<SameLine>(m_meshLayout);
    emplaceComponent<Label>(m_meshLayout, "Z:", defaultTextConfig);
    m_posXInputBox = emplaceComponent<InputBox<float>>(m_meshLayout, "PosXInputBox", inputBoxConfig);
    emplaceComponent<SameLine>(m_meshLayout);
    m_posYInputBox = emplaceComponent<InputBox<float>>(m_meshLayout, "PosYInputBox", inputBoxConfig);
    emplaceComponent<SameLine>(m_meshLayout);
    m_posZInputBox = emplaceComponent<InputBox<float>>(m_meshLayout, "PosZInputBox", inputBoxConfig);
    emplaceComponent<Separator>(m_meshLayout, 0.55f);
}

void AdditionLayer::initSurfaceComponents()
{
    using namespace ui::components;

    static constexpr int mainItemsCount = 23;

    auto headlinersConfig = LabelConfigBuilder()
        .font(ui::styling::Font::extraBold(0.025f))
        .build();

    auto defaultTextConfig = LabelConfigBuilder()
        .font(ui::styling::Font::regular(0.02))
        .build();

    auto inputBoxConfig = InputBoxBuilder()
        .width(0.17f)
        .background(ui::styling::Color::lightGray)
        .build();

    auto apiKeyConfig = InputBoxBuilder()
        .width(0.35f)
        .background(ui::styling::Color::lightGray)
        .build();

    m_surfaceLayout = emplaceLayout(ImVec2{ 0.2f, 0.05f });
    m_surfaceLayout->reserveComponents(mainItemsCount);

    emplaceComponent<Label>(m_surfaceLayout, "Geographic Bounds", headlinersConfig);
    emplaceComponent<Dummy>(m_surfaceLayout, 0.13f, 0.0f);
    emplaceComponent<SameLine>(m_surfaceLayout);
    emplaceComponent<Label>(m_surfaceLayout, "Longitude", defaultTextConfig);
    emplaceComponent<SameLine>(m_surfaceLayout);
    emplaceComponent<Dummy>(m_surfaceLayout, 0.01f, 0.0f);
    emplaceComponent<SameLine>(m_surfaceLayout);
    emplaceComponent<Label>(m_surfaceLayout, "Latitude", defaultTextConfig);
    emplaceComponent<Dummy>(m_surfaceLayout, 0.02f, 0.0f);
    emplaceComponent<SameLine>(m_surfaceLayout);
    emplaceComponent<Label>(m_surfaceLayout, "LL:", defaultTextConfig);
    emplaceComponent<SameLine>(m_surfaceLayout);
    emplaceComponent<Dummy>(m_surfaceLayout, 0.02f, 0.0f);
    emplaceComponent<SameLine>(m_surfaceLayout);
    m_lowerLeftLon = emplaceComponent<InputBox<float>>(m_surfaceLayout, "LL_Lon", inputBoxConfig);
    emplaceComponent<SameLine>(m_surfaceLayout);
    m_lowerLeftLat = emplaceComponent<InputBox<float>>(m_surfaceLayout, "LL_Lat", inputBoxConfig);
    emplaceComponent<Dummy>(m_surfaceLayout, 0.02f, 0.0f);
    emplaceComponent<SameLine>(m_surfaceLayout);
    emplaceComponent<Label>(m_surfaceLayout, "UR:", defaultTextConfig);
    emplaceComponent<SameLine>(m_surfaceLayout);
    emplaceComponent<Dummy>(m_surfaceLayout, 0.01f, 0.0f);
    emplaceComponent<SameLine>(m_surfaceLayout);
    m_upperRightLon = emplaceComponent<InputBox<float>>(m_surfaceLayout, "UR_Lon", inputBoxConfig);
    emplaceComponent<SameLine>(m_surfaceLayout);
    m_upperRightLat = emplaceComponent<InputBox<float>>(m_surfaceLayout, "UR_Lat", inputBoxConfig);
    emplaceComponent<Separator>(m_surfaceLayout, 0.55f);
    emplaceComponent<Label>(m_surfaceLayout, "API Configuration", headlinersConfig);
    emplaceComponent<Dummy>(m_surfaceLayout, 0.19f, 0.0f);
    emplaceComponent<SameLine>(m_surfaceLayout);
    emplaceComponent<Label>(m_surfaceLayout, "API Key:", defaultTextConfig);
    emplaceComponent<Dummy>(m_surfaceLayout, 0.08f, 0.0f);
    emplaceComponent<SameLine>(m_surfaceLayout);    
    m_apiKeyBuffer = emplaceComponent<InputBox<char>>(m_surfaceLayout, "APIKeyInput", apiKeyConfig);
    emplaceComponent<Separator>(m_surfaceLayout, 0.55f);
}

void AdditionLayer::handleComponentsVisibility()
{
    m_subdivisionInputBox->setIsVisible(!*m_automaticCheckBox->isChecked());
    m_subdivisionSlider->setIsVisible(!*m_automaticCheckBox->isChecked());
}

template <typename TParams>
void AdditionLayer::setCreateButton(const std::string& commandId)
{
    using namespace ui::components;

    m_createButton->setAction([this, commandId](Button*) {
        auto* command = CommandRegistry::instance().getCommand(commandId);
        if (command != nullptr)
        {
            TParams params;
            params.m_subdivisionLevel = *m_subdivisionInputBox->inputValue();
            params.m_size             = *m_sizeInputBox->inputValue();
            params.m_position         = glm::vec3{
                *m_posXInputBox->inputValue(),
                *m_posZInputBox->inputValue(),
                *m_posYInputBox->inputValue()
            };
            
            command->execute(params);

            VisibilityHandler::hide(LAYER_NAME);
        }
    });
}

void AdditionLayer::setCreateButtonAsAddSurface()
{
    using namespace ui::components; 

    m_createButton->setAction([this](Button*) {
        auto* command = CommandRegistry::instance().getCommand("ADD_SURFACE_COMMAND");
        if (command != nullptr)
        {
            OpenTopoParams params;
            params.m_lowerLeftLon  = *m_lowerLeftLon->inputValue();
            params.m_lowerLeftLat  = *m_lowerLeftLat->inputValue();
            params.m_upperRightLon = *m_upperRightLon->inputValue();
            params.m_upperRightLat = *m_upperRightLat->inputValue();
            params.m_apiKey        = *m_apiKeyBuffer->inputValue();
            
            command->execute(params);

            VisibilityHandler::hide(LAYER_NAME);
        }
    });
}

void AdditionLayer::onImGuiRender()
{
    m_meshLayout->setIsVisible(false);
    m_surfaceLayout->setIsVisible(false);

    switch (m_additionType)
    {
        case AdditionType::PLANE:
            setCreateButton<PlaneParams>("ADD_PLANE_COMMAND");
            m_meshLayout->setIsVisible(true);
            break;
        case AdditionType::CUBE:
            setCreateButton<CubeParams>("ADD_CUBE_COMMAND");
            m_meshLayout->setIsVisible(true);
            break;
        case AdditionType::SURFACE:
            setCreateButtonAsAddSurface();
            m_surfaceLayout->setIsVisible(true);
            break;
        default:
            break;
    }

    handleComponentsVisibility();

    this->render();
}