#include "AdditionLayer.h"
#include "imgui.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/UI/Components/Button.h"
#include "../../src/UI/Styling/Font.h"
#include "../../src/Builders/UIWindowBuilder.h"
#include "../../src/Builders/UIButtonBuilder.h"
#include "../../src/Builders/UILabelBuilder.h"
#include "../../src/Builders/UICheckBoxBuilder.h"
#include "../../src/Builders/UIInputBoxBuilder.h"
#include "../src/UI/Components/Label.h"
#include "../src/UI/Components/CheckBox.h"
#include "../src/UI/Components/InputBox.h"

static AutoRegisterLayerArgs<AdditionLayer, std::string> reg("ADDITION_LAYER");

AdditionLayer::AdditionLayer(const std::string& name)
    : Layer(name)
    , OverlappingWindow(initWindowConfig())
    , m_additionType(AdditionType::NONE)
{
    initComponents();
}

ui::styling::WindowConfig AdditionLayer::initWindowConfig()
{
    auto posConfig = WindowPosConfigBuilder()
        .posX(0.38f)
        .posY(0.28f)
        .build();

    auto sizeConfig = WindowSizeConfigBuilder()
        .width(0.25f)
        .height(0.35f)
        .minWidth(0.6f)
        .minHeight(0.6f)
        .build();

    auto titleBarConfig = WindowTitleBarBuilder()
        .background(ui::styling::Color::titleBarOrange)
        .font(ui::styling::Font::extraBold(20.0f))
        .build();

    auto flags = ImGuiWindowFlags_NoResize
               | ImGuiWindowFlags_NoMove
               | ImGuiWindowFlags_NoScrollbar
               | ImGuiWindowFlags_NoCollapse;
    
    return WindowConfigBuilder()
        .name("\tEnter parameters")
        .layerName("ADDITION_LAYER")
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
    auto headlinersConfig = LabelConfigBuilder()
        .font(ui::styling::Font::extraBold(0.025f))
        .build();

    auto defaultTextConfig = LabelConfigBuilder()
        .font(ui::styling::Font::regular(0.02))
        .build();

    auto checkBoxConfig = CheckBoxConfigBuilder()
        .checkMarkColor(ui::styling::Color::selectedOrange)
        .background(ui::styling::Color::darkGray)
        .onHoverBackground(ui::styling::Color::darkGray)
        .onActiveBackground(ui::styling::Color::darkGray)
        .build();

    auto inputBoxConfig = InputBoxBuilder()
        .width(0.17f)
        .background(ui::styling::Color::darkGray)
        .build();

    m_subdivisionLabel    = std::make_unique<ui::components::Label>("Subdivision", headlinersConfig);
    m_sizeLabel           = std::make_unique<ui::components::Label>("Size", headlinersConfig);
    m_positionLabel       = std::make_unique<ui::components::Label>("Position", headlinersConfig);
    m_automaticLabel      = std::make_unique<ui::components::Label>("Automatic:", defaultTextConfig);
    m_posXLabel           = std::make_unique<ui::components::Label>("X:", defaultTextConfig);
    m_posYLabel           = std::make_unique<ui::components::Label>("Y:", defaultTextConfig);
    m_posZLabel           = std::make_unique<ui::components::Label>("Z:", defaultTextConfig);
    m_automaticCheckbox   = std::make_unique<ui::components::CheckBox>("AutomaticCheckBox", checkBoxConfig);
    m_subdivisionInputBox = std::make_unique<ui::components::InputBox<int>>("SubdivisionInputBox", inputBoxConfig);
    m_sizeInputBox        = std::make_unique<ui::components::InputBox<float>>("SizeInputBox", inputBoxConfig);
    m_posXInputBox        = std::make_unique<ui::components::InputBox<float>>("PosXInputBox", inputBoxConfig);
    m_posYInputBox        = std::make_unique<ui::components::InputBox<float>>("PosYInputBox", inputBoxConfig);
    m_posZInputBox        = std::make_unique<ui::components::InputBox<float>>("PosZInputBox", inputBoxConfig);

    *m_subdivisionInputBox->inputValue() = 1;
    *m_sizeInputBox->inputValue()        = 1;
}

void AdditionLayer::addPlane()
{
    auto* addPlaneCommand = CommandRegistry::instance().getCommand("ADD_PLANE_COMMAND");

    if (/*addPlaneCommand*/ true)
    {
        drawMeshComponents(addPlaneCommand, [this]() -> PlaneParams {
            PlaneParams addPlaneCommandParams;
            addPlaneCommandParams.m_subdivisionLevel = *m_subdivisionInputBox->inputValue();
            addPlaneCommandParams.m_size             = *m_sizeInputBox->inputValue();
            addPlaneCommandParams.m_position = glm::vec3{
                *m_posXInputBox->inputValue(),
                *m_posZInputBox->inputValue(),
                *m_posYInputBox->inputValue()
            };
            
            return addPlaneCommandParams;
        });
    }
}

void AdditionLayer::addCube()
{
    auto* addCubeCommand = CommandRegistry::instance().getCommand("ADD_CUBE_COMMAND");

    if (/*addCubeCommand*/ true)
    {
        drawMeshComponents(addCubeCommand, [this]() -> CubeParams {
            CubeParams addCubeParams;
            addCubeParams.m_size             = *m_sizeInputBox->inputValue();
            addCubeParams.m_subdivisionLevel = *m_subdivisionInputBox->inputValue();
            addCubeParams.m_position = glm::vec3{
                *m_posXInputBox->inputValue(),
                *m_posZInputBox->inputValue(),
                *m_posYInputBox->inputValue()
            };
            
            return addCubeParams;
        });
    }
}

void AdditionLayer::addSurface()
{
    auto* addSurfaceCommand = CommandRegistry::instance().getCommand("ADD_SURFACE_COMMAND");

    if (/*addSurfaceCommand*/ true)
    {
        drawSurfaceComponents(addSurfaceCommand);
    }
}

void AdditionLayer::drawMeshComponents(CommandConcept* command, std::function<CommandParams()> paramsCallback)
{
    ui::styling::Window::addToLayout([this]() {
        // Subdivision
        ui::styling::Label::init(m_subdivisionLabel->text(), m_subdivisionLabel->config());
        ui::styling::Label::destroy(m_subdivisionLabel->config());

        ui::styling::Label::init(m_automaticLabel->text(), m_automaticLabel->config());
        ui::styling::Label::destroy(m_automaticLabel->config());
        
        ImGui::SameLine();

        ui::styling::CheckBox::init(m_automaticLabel->text(), m_automaticCheckbox->isChecked(), m_automaticCheckbox->config());
        ui::styling::CheckBox::destroy(m_automaticCheckbox->config());

        if (!*m_automaticCheckbox->isChecked())
        {
            ui::styling::InputBox::init(m_subdivisionInputBox->name(), m_subdivisionInputBox->inputValue(), m_subdivisionInputBox->config());
            ui::styling::InputBox::destroy(m_subdivisionInputBox->config());
        }
        else
        {
            *m_subdivisionInputBox->inputValue() = 1;
            ImGui::Dummy(ImVec2{ 0.0f, ImGui::GetFrameHeight() });
        }

        ui::styling::Window::drawHorizontalSeparator(0.55f);

        // Size
        ui::styling::Label::init(m_sizeLabel->text(), m_sizeLabel->config());
        ui::styling::Label::destroy(m_sizeLabel->config());

        ui::styling::InputBox::init(m_sizeInputBox->name(), m_sizeInputBox->inputValue(), m_sizeInputBox->config());
        ui::styling::InputBox::destroy(m_sizeInputBox->config());

        ui::styling::Window::drawHorizontalSeparator(0.55f);

        // Position
        ui::styling::Label::init(m_positionLabel->text(), m_positionLabel->config());
        ui::styling::Label::destroy(m_positionLabel->config());
        ImGui::Dummy(ImVec2{ 0.003f * ImGui::GetWindowSize().x, 0.0f });
        ImGui::SameLine();

        ui::styling::Label::init(m_posXLabel->text(), m_posXLabel->config());
        ui::styling::Label::destroy(m_posXLabel->config());
        ImGui::SameLine();
        ImGui::Dummy(ImVec2{ 0.13f * ImGui::GetWindowSize().x, 0.0f });
        ImGui::SameLine();

        ui::styling::Label::init(m_posYLabel->text(), m_posYLabel->config());
        ui::styling::Label::destroy(m_posYLabel->config());
        ImGui::SameLine();
        ImGui::Dummy(ImVec2{ 0.13f * ImGui::GetWindowSize().x, 0.0f });
        ImGui::SameLine();

        ui::styling::Label::init(m_posZLabel->text(), m_posZLabel->config());
        ui::styling::Label::destroy(m_posZLabel->config());

        ui::styling::InputBox::init(m_posXInputBox->name(), m_posXInputBox->inputValue(), m_posXInputBox->config());
        ui::styling::InputBox::destroy(m_posXInputBox->config());
        ImGui::SameLine();

        ui::styling::InputBox::init(m_posYInputBox->name(), m_posYInputBox->inputValue(), m_posYInputBox->config());
        ui::styling::InputBox::destroy(m_posYInputBox->config());
        ImGui::SameLine();

        ui::styling::InputBox::init(m_posZInputBox->name(), m_posZInputBox->inputValue(), m_posZInputBox->config());
        ui::styling::InputBox::destroy(m_posZInputBox->config());

        ui::styling::Window::drawHorizontalSeparator(0.55f);
    }, ImVec2{ 0.2f, 0.05f });

    ui::styling::Window::addToLayout([]() {
        //TODO
    });
}

void AdditionLayer::drawSurfaceComponents(CommandConcept* command)
{
    //TODO
}

void AdditionLayer::onImGuiRender()
{
    ui::styling::Window::setPosAndSize(m_windowConfig);
    if (!VisibilityHandler::isVisible("ADDITION_LAYER"))
    {
        return;
    }

    ui::styling::Window::init(m_windowConfig);

    switch (m_additionType)
    {
        case AdditionType::PLANE:
            addPlane();
            break;
        case AdditionType::CUBE:
            addCube();
            break;
        case AdditionType::SURFACE:
            addSurface();
            break;
        default:
            break;
    }

    ui::styling::Window::destroy(m_windowConfig);
}