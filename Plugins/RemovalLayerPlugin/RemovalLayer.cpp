#include "RemovalLayer.h"
#include "imgui.h"
#include "../../src/Commands/CommandRegistry.h"

static AutoRegisterLayerArgs<RemovalLayer, std::string> reg("REMOVAL_LAYER");

RemovalLayer::RemovalLayer(const std::string &name)
    : Layer(name) {}
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

void RemovalLayer::onImGuiRender()
{
    ImGui::Begin(this->getName().c_str());

    // Get window position and size
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 mousePos = ImGui::GetMousePos();

    // Update the class variable to track if the mouse is inside the window
    m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                             mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);


    auto *deleteSelectedFacesCommand = CommandRegistry::instance().getCommand("DELETE_SELECTED_FACES_COMMAND");
    if(deleteSelectedFacesCommand)
    {
        if (ImGui::Button("Delete Selected Faces"))
        {
            deleteSelectedFacesCommand->execute();
        }
    }

    auto *deleteSelectedMeshesCommand = CommandRegistry::instance().getCommand("DELETE_SELECTED_MESHES_COMMAND");
    if(deleteSelectedMeshesCommand)
    {
    if (ImGui::Button("Delete Selected Meshes"))
    {
        deleteSelectedMeshesCommand->execute();
    }

    }

    ImGui::End();
}
