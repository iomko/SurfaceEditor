#include "AdditionLayer.h"
#include "imgui.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/UI/Components/Button.h"
#include "../../src/UI/Styling/Font.h"
#include "../src/UI/Components/Label.h"
#include "../src/UI/Components/CheckBox.h"
#include "../src/UI/Components/InputBox.h"
#include "../src/UI/Components/Slider.h"
#include "../src/UI/Components/SameLine.h"
#include "../src/UI/Components/Separator.h"
#include "../src/UI/Components/Dummy.h"
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
}

void AdditionLayer::initMeshComponents()
{
    using namespace ui::components;

    static constexpr int mainItemsCount   = 31;
    static constexpr int dialogItemsCount = 3;
    
    auto buttonSize = ImVec2{
        m_windowConfig.size.width * 0.5f,
        m_windowConfig.size.height * 0.25f
    };

    auto dialogButtonConfig = ButtonConfigBuilder()
        .rounding(8.0f)
        .size(buttonSize)
        .background(ui::styling::Color::lightGray)
        .font(ui::styling::Font::regular(0.6f))
        .size(ImVec2{ 0.042f, 0.04f })
        .onHoverColor(ui::styling::Color::hoverOverOrange)
        .onClickColor(ui::styling::Color::onClickOrange)
        .build();

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

    ui::Layout* mainLayout = emplaceLayout(ImVec2{ 0.2f, 0.05f });
    mainLayout->reserveComponents(mainItemsCount);

    emplaceComponent<Label>(mainLayout, "Subdivision", headlinersConfig);
    emplaceComponent<Label>(mainLayout, "Automatic:", defaultTextConfig);
    emplaceComponent<SameLine>(mainLayout);
    m_automaticCheckBox   = emplaceComponent<CheckBox>(mainLayout, "AutomaticCheckBox", checkBoxConfig);
    m_subdivisionInputBox = emplaceComponent<InputBox<int>>(mainLayout, "SubdivisionInputBox", inputBoxConfig);
    emplaceComponent<SameLine>(mainLayout);
    m_subdivisionSlider = emplaceComponent<Slider<int>>(mainLayout, "SubdivisionSlider", intSliderConfig);
    emplaceComponent<Separator>(mainLayout, 0.55f);
    emplaceComponent<Label>(mainLayout, "Size", headlinersConfig);
    m_sizeInputBox = emplaceComponent<InputBox<float>>(mainLayout, "SizeInputBox", inputBoxConfig);
    emplaceComponent<SameLine>(mainLayout);
    m_sizeSlider = emplaceComponent<Slider<float>>(mainLayout, "SizeSlider", floatSliderConfig);
    emplaceComponent<Separator>(mainLayout, 0.55f);
    emplaceComponent<Label>(mainLayout, "Position", headlinersConfig);
    emplaceComponent<Dummy>(mainLayout, 0.003f, 0.0f);
    emplaceComponent<SameLine>(mainLayout);
    emplaceComponent<Label>(mainLayout, "X:", defaultTextConfig);
    emplaceComponent<SameLine>(mainLayout);
    emplaceComponent<Dummy>(mainLayout, 0.13f, 0.0f);
    emplaceComponent<SameLine>(mainLayout);
    emplaceComponent<Label>(mainLayout, "Y:", defaultTextConfig);
    emplaceComponent<SameLine>(mainLayout);
    emplaceComponent<Dummy>(mainLayout, 0.13f, 0.0f);
    emplaceComponent<SameLine>(mainLayout);
    emplaceComponent<Label>(mainLayout, "Z:", defaultTextConfig);
    m_posXInputBox = emplaceComponent<InputBox<float>>(mainLayout, "PosXInputBox", inputBoxConfig);
    emplaceComponent<SameLine>(mainLayout);
    m_posYInputBox = emplaceComponent<InputBox<float>>(mainLayout, "PosYInputBox", inputBoxConfig);
    emplaceComponent<SameLine>(mainLayout);
    m_posZInputBox = emplaceComponent<InputBox<float>>(mainLayout, "PosZInputBox", inputBoxConfig);
    emplaceComponent<Separator>(mainLayout, 0.55f);

    ui::Layout* dialogLayout = emplaceLayout(ImVec2{ 0.28f, 0.0f });
    dialogLayout->reserveComponents(dialogItemsCount);

    m_createButton = emplaceComponent<Button>(dialogLayout, "Add", dialogButtonConfig, [](Button*){});
    emplaceComponent<SameLine>(dialogLayout);
    emplaceComponent<Button>(dialogLayout, "Cancel", dialogButtonConfig, [](Button*) {
        VisibilityHandler::hide("ADDITION_LAYER");
    });

    m_subdivisionSlider->linkWith(m_subdivisionInputBox);
    m_sizeSlider->linkWith(m_sizeInputBox);

    *m_subdivisionInputBox->inputValue() = 1;
    *m_sizeInputBox->inputValue()        = 1.0f;
}

void AdditionLayer::initSurfaceComponents()
{
    using namespace ui::components;

    //TODO
}

void AdditionLayer::handleComponentsVisibility()
{
    m_subdivisionInputBox->setIsVisible(!*m_automaticCheckBox->isChecked());
    m_subdivisionSlider->setIsVisible(!*m_automaticCheckBox->isChecked());
}

void AdditionLayer::setCreateButtonAsAddPlane()
{
    using namespace ui::components;

    // m_createButton->setAction([this](Button*) {
    //     auto* command = CommandRegistry::instance().getCommand("ADD_PLANE_COMMAND");
    //     if (command != nullptr)
    //     {
    //         PlaneParams params;
    //         params.m_subdivisionLevel = *m_subdivisionInputBox->inputValue();
    //         params.m_size             = *m_sizeInputBox->inputValue();
    //         params.m_position = glm::vec3{
    //             *m_posXInputBox->inputValue(),
    //             *m_posZInputBox->inputValue(),
    //             *m_posYInputBox->inputValue()
    //         };
            
    //         command->execute(params);
    //     }
    // });
}

void AdditionLayer::setCreateButtonAsAddCube()
{
    using namespace ui::components;

    // m_createButton->setAction([this](Button*) {
    //     auto* command = CommandRegistry::instance().getCommand("ADD_CUBE_COMMAND");   
    //     if (command != nullptr)
    //     {
    //         CubeParams params;
    //         params.m_size             = *m_sizeInputBox->inputValue();
    //         params.m_subdivisionLevel = *m_subdivisionInputBox->inputValue();
    //         params.m_position = glm::vec3{
    //             *m_posXInputBox->inputValue(),
    //             *m_posZInputBox->inputValue(),
    //             *m_posYInputBox->inputValue()
    //         };
                
    //         command->execute(params);
    //     }
    // });
}

void AdditionLayer::setCreateButtonAsAddSurface()
{
    using namespace ui::components; 

    // m_createButton->setAction([this](Button*) {
    //     auto* command = CommandRegistry::instance().getCommand("ADD_SURFACE_COMMAND");
    //     if (command != nullptr)
    //     {
    //         OpenTopoParams params;
    //         // params.m_lowerLeftLon = m_lowerLeftLon;
    //         // params.m_lowerLeftLat = m_lowerLeftLat;
    //         // params.m_upperRightLon = m_upperRightLon;
    //         // params.m_upperRightLat = m_upperRightLat;
    //         // params.m_apiKey = std::string(m_apiKeyBuffer);
            
    //         command->execute(params);
    //     }
    // });
}

void AdditionLayer::onImGuiRender()
{
    switch (m_additionType)
    {
        case AdditionType::PLANE:
            setCreateButtonAsAddPlane();
            break;
        case AdditionType::CUBE:
            setCreateButtonAsAddCube();
            break;
        case AdditionType::SURFACE:
            setCreateButtonAsAddSurface();
            break;
        default:
            break;
    }

    handleComponentsVisibility();

    this->render();
}