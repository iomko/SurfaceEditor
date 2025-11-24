#include "RemovalLayer.h"
#include "LayerIDs.h"
#include "LayerRegistry.h"
#include "imgui.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"

static AutoRegisterLayer<RemovalLayer> reg(REMOVAL_LAYER);

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

    if (ImGui::Button("Delete Selected Faces"))
    {
        auto *deleteSelectedFacesCommand = CommandRegistry::instance().getCommand(DELETE_SELECTED_FACES_COMMAND);
        if (deleteSelectedFacesCommand)
            deleteSelectedFacesCommand->execute();
    }

    if (ImGui::Button("Delete Selected Meshes"))
    {
        auto *deleteSelectedMeshesCommand = CommandRegistry::instance().getCommand(DELETE_SELECTED_MESHES_COMMAND);
        if (deleteSelectedMeshesCommand)
            deleteSelectedMeshesCommand->execute();
    }

    ImGui::End();
}
