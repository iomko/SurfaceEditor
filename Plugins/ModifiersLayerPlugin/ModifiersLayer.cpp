#include "ModifiersLayer.h"
#include "imgui.h"
#include "../../src/Commands/CommandRegistry.h"
#include <Builders/UIWindowBuilder.h>
#include <Builders/UIButtonBuilder.h>
#include <Styling/Font.h>
#include <Utils/VisibilityHandler.h>
#include <Components/Separator.h>
#include <Components/Dummy.h>
#include <Components/SameLine.h>

namespace
{
    static constexpr const char *LAYER_NAME = "MODIFIERS_LAYER";
}

static AutoRegisterLayerArgs<ModifiersLayer,std::string, std::reference_wrapper<WindowLayerBus>> reg("MODIFIERS_LAYER");

ModifiersLayer::ModifiersLayer(const std::string &name, WindowLayerBus &windowLayerBus)
    : Layer(name), ui::components::Window(LAYER_NAME)
{
    windowLayerBus.on<OutlinerLayerState>([&](OutlinerLayerState &outlinerLayerState)
                                          {
                if(OutlinerNode<Mesh>* node = dynamic_cast<OutlinerNode<Mesh>*>(outlinerLayerState.m_currentSelectedNode)) {
                    m_state.m_selectedMesh = node->m_data;
                } else {
                    std::cout << "NOT DYNAMIC CAST" << std::endl;
                    m_state.m_selectedMesh = nullptr;
                }; });
    VisibilityHandler::show(LAYER_NAME);
}

void ModifiersLayer::onEvent(Event &event)
{
    if (event.getType() == EventType::MouseButtonPress)
    {
        if (m_state.m_isMouseInsideWindow)
        {
            event.isHandled = true;
        }
    }
}

void ModifiersLayer::initWindowConfig() 
{
    auto posConfig = WindowPosConfigBuilder()
                         .posX(0.12f)
                         .posY(0)
                         .build();

    auto sizeConfig = WindowSizeConfigBuilder()
                          .width(0.09f)
                          .height(0.19f)
                          .minWidth(0.01f)
                          .minHeight(0.01f)
                          .build();

    auto titleBarConfig = WindowTitleBarBuilder()
                              .background(ui::styling::Color::titleBarOrange)
                              .font(ui::styling::Font::extraBold(20.0f))
                              .build();

    auto flags = ImGuiWindowFlags_NoResize;

    m_windowConfig = WindowConfigBuilder()
                         .name("Modifiers layer")
                         .layerName(LAYER_NAME)
                         .background(ui::styling::Color::transparentGray)
                         .titleBar(titleBarConfig)
                         .pos(posConfig)
                         .size(sizeConfig)
                         .flags(flags)
                         .build();
}

void ModifiersLayer ::initComponents()
{
    auto btnConfig = ButtonConfigBuilder()
                         .rounding(8.0f)
                         .background(ui::styling::Color::lightGray)
                         .font(ui::styling::Font::regular(0.6f))
                         .size(ImVec2{0.042f, 0.04f})
                         .onHoverColor(ui::styling::Color::hoverOverOrange)
                         .onClickColor(ui::styling::Color::onClickOrange)
                         .build();

    auto placeHolder = [](ui::components::Button *) {};

    ImVec2 dummySize = ImVec2(0.5f, 0.02f);

    ui::components::Layout *layout = emplaceLayout();
    layout->reserveComponents(12);
    emplaceComponent<ui::components::Dummy>(layout,(this->m_windowConfig.size.width+this->m_windowConfig.pos.posX)/1.3f,0.04f);
    emplaceComponent<ui::components::SameLine>(layout);
    m_solidifyButton = emplaceComponent<ui::components::Button>(layout, "Solidify", btnConfig, [this](ui::components::Button *)
                                                              { meshesCommandClicked = !meshesCommandClicked; });
    emplaceComponent<ui::components::Dummy>(layout, dummySize.x, dummySize.y);
    emplaceComponent<ui::components::Separator>(layout, 1);
    emplaceComponent<ui::components::Dummy>(layout, dummySize.x, dummySize.y);
    emplaceComponent<ui::components::Dummy>(layout,(this->m_windowConfig.size.width+this->m_windowConfig.pos.posX)/1.3f,0.04f);
    emplaceComponent<ui::components::SameLine>(layout);
    m_createPrintButton = emplaceComponent<ui::components::Button>(layout, "Print", btnConfig, [this](ui::components::Button *)
                                                              { printCommandClicked = !printCommandClicked; });
    emplaceComponent<ui::components::Dummy>(layout, dummySize.x, dummySize.y - 0.01f);
    emplaceComponent<ui::components::Dummy>(layout, dummySize.x, dummySize.y);
    emplaceComponent<ui::components::Separator>(layout, 1);
}

void ModifiersLayer::onImGuiRender()
{
    
    this->render();

    // Get window position and size
    ImVec2 windowPos = this->m_windowConfig.pos.rawPos;
    ImVec2 windowSize = this->m_windowConfig.size.rawSize;
    ImVec2 mousePos = ImGui::GetMousePos();

    m_state.m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                                     mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

    isMouseOnHeader = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                        mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + (windowSize.y*0.25));
    
    auto *solidifyMeshesCommand = CommandRegistry::instance().getCommand("SOLIDIFY_MESHES_COMMAND");
    if(solidifyMeshesCommand)
    {
        if (meshesCommandClicked) {
            solidifyMeshesCommand->execute();
        }
    }
    auto *createPrintCommand = CommandRegistry::instance().getCommand("CREATE_PRINT_COMMAND");
    if(createPrintCommand)
    {
        if (createPrintCommand)
        {
            if (m_state.m_selectedMesh != nullptr)
            {            
                PrintMeshSettingsParams printMeshSettingsParams;
                printMeshSettingsParams.mesh = m_state.m_selectedMesh;
                printMeshSettingsParams.height = 1.0f;
                createPrintCommand->execute(printMeshSettingsParams);
            }
        }
    }
}