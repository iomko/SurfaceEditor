#include "RemovalLayer.h"
#include "imgui.h"
#include "../../src/Commands/CommandRegistry.h"
#include <Builders/UIWindowBuilder.h>
#include <Builders/UIButtonBuilder.h>
#include <Styling/Font.h>
#include <Utils/VisibilityHandler.h>
#include <Components/Separator.h>
#include <Components/Dummy.h>

namespace
{
    static constexpr const char *LAYER_NAME = "REMOVAL_LAYER";
}

static AutoRegisterLayerArgs<RemovalLayer, std::string> reg("REMOVAL_LAYER");

RemovalLayer::RemovalLayer(const std::string &name)
    : Layer(name), ui::components::Window(LAYER_NAME) {
        VisibilityHandler::show(LAYER_NAME);
    }

void RemovalLayer::onEvent(Event &event)
{
    if (event.getType() == EventType::MouseButtonPress)
    {
        if (m_isMouseInsideWindow)
        {
            event.isHandled = true;
        }
    }
}

void RemovalLayer::initWindowConfig() 
{
    auto posConfig = WindowPosConfigBuilder()
                         .posX(0.21f)
                         .posY(0)
                         .build();

    auto sizeConfig = WindowSizeConfigBuilder()
                          .width(0.15f)
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
                         .name("Removal layer")
                         .layerName(LAYER_NAME)
                         .background(ui::styling::Color::transparentGray)
                         .titleBar(titleBarConfig)
                         .pos(posConfig)
                         .size(sizeConfig)
                         .flags(flags)
                         .build();
}

void RemovalLayer::initComponents()
{
    auto btnConfig = ButtonConfigBuilder()
                         .rounding(8.0f)
                         .background(ui::styling::Color::lightGray)
                         .font(ui::styling::Font::regular(0.6f))
                         .size(ImVec2{0.14f, 0.04f})
                         .onHoverColor(ui::styling::Color::hoverOverOrange)
                         .onClickColor(ui::styling::Color::onClickOrange)
                         .build();

    auto placeHolder = [](ui::components::Button *) {};

    ImVec2 dummySize = ImVec2(0.5f, 0.02f);

    ui::components::Layout *layout = emplaceLayout();
    layout->reserveComponents(8);
    emplaceComponent<ui::components::Button>(layout, "Delete selected faces", btnConfig, [this](ui::components::Button *)
                                                              { deleteSelectedFaces = !deleteSelectedFaces; });
    emplaceComponent<ui::components::Dummy>(layout, dummySize.x, dummySize.y);
    emplaceComponent<ui::components::Separator>(layout, 1);
    emplaceComponent<ui::components::Dummy>(layout, dummySize.x, dummySize.y);
    emplaceComponent<ui::components::Button>(layout, "Delete selected meshes", btnConfig, [this](ui::components::Button *)
                                                              { deleteSelectedFaces = !deleteSelectedMeshes; });
    emplaceComponent<ui::components::Dummy>(layout, dummySize.x, dummySize.y - 0.01f);
    emplaceComponent<ui::components::Dummy>(layout, dummySize.x, dummySize.y);
    emplaceComponent<ui::components::Separator>(layout, 1);
}

void RemovalLayer::onImGuiRender()
{
    
    this->render();

    // Get window position and size
    ImVec2 windowPos = this->m_windowConfig.pos.rawPos;
    ImVec2 windowSize = this->m_windowConfig.size.rawSize;
    ImVec2 mousePos = ImGui::GetMousePos();

    // Update the class variable to track if the mouse is inside the window
    m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                             mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

    isMouseOnHeader = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                        mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + (windowSize.y*0.25));


    auto *deleteSelectedFacesCommand = CommandRegistry::instance().getCommand("DELETE_SELECTED_FACES_COMMAND");
    if(deleteSelectedFacesCommand)
    {
        if (deleteSelectedFaces)
        {
            deleteSelectedFacesCommand->execute();
        }
    }

    auto *deleteSelectedMeshesCommand = CommandRegistry::instance().getCommand("DELETE_SELECTED_MESHES_COMMAND");
    if(deleteSelectedMeshes)
    {
    if (ImGui::Button("Delete Selected Meshes"))
    {
        deleteSelectedMeshesCommand->execute();
    }

    }
}
