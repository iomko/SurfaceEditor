#include "AdditionLayer.h"
#include "imgui.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/UI/Components/Button.h"
#include "../../src/UI/Styling/Font.h"
#include "../../src/Builders/UIWindowBuilder.h"
#include "../../src/Builders/UIButtonBuilder.h"
#include "../../src/Builders/UILabelBuilder.h"
#include "../src/UI/Components/Label.h"

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
    auto posConfig      = WindowPosConfigBuilder().posX(0.38f).posY(0.3f).build();
    auto sizeConfig     = WindowSizeConfigBuilder().width(0.25f).height(0.35f).minWidth(0.6f).minHeight(0.6f).build();
    auto titleBarConfig = WindowTitleBarBuilder().background(ui::styling::Color::titleBarOrange).font(ui::styling::Font::extraBold(20.0f)).build();
    auto flags          = ImGuiWindowFlags_NoResize
                        | ImGuiWindowFlags_NoMove
                        | ImGuiWindowFlags_NoScrollbar
                        | ImGuiWindowFlags_NoCollapse;
    
    return WindowConfigBuilder()
        .name("\tEnter parameters")
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
        .font(ui::styling::Font::extraBold(15.0f))
        .build();

    m_subdivisionLabel = std::make_unique<ui::components::Label>("Subdivision", headlinersConfig);
    m_sizeLabel        = std::make_unique<ui::components::Label>("Size", headlinersConfig);
    m_positionLabel    = std::make_unique<ui::components::Label>("Position", headlinersConfig);
}

void AdditionLayer::addPlane()
{
    auto* addPlaneCommand = CommandRegistry::instance().getCommand("ADD_PLANE_COMMAND");

    if (/*addPlaneCommand*/ true)
    {
        drawMeshComponents(addPlaneCommand, [this]() -> PlaneParams {
            PlaneParams addPlaneCommandParams;
            addPlaneCommandParams.m_subdivisionLevel = m_subdivision;
            addPlaneCommandParams.m_size             = m_size;
            addPlaneCommandParams.m_position         = glm::vec3(m_xPos, m_zPos, m_yPos);
            
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
            addCubeParams.m_size = m_size;
            addCubeParams.m_subdivisionLevel = m_subdivision;
            addCubeParams.m_position = glm::vec3(m_xPos, m_zPos, m_yPos);
            
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
        ui::styling::Window::drawHorizontalSeparator(0.7f);

        // Size
        ui::styling::Label::init(m_sizeLabel->text(), m_sizeLabel->config());
        ui::styling::Label::destroy(m_sizeLabel->config());
        ui::styling::Window::drawHorizontalSeparator(0.7f);

        // Position
        ui::styling::Label::init(m_positionLabel->text(), m_positionLabel->config());
        ui::styling::Label::destroy(m_positionLabel->config());
        ui::styling::Window::drawHorizontalSeparator(0.7f);

        // Dialog
        //TODO
    }, ImVec2{ 0.2f, 0.1f });
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