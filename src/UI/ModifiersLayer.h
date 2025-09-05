#pragma once
#include <string>
#include "Commands/SolidifyMeshesCommand.h"
#include "imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"
#include "../Commands/FetchSurfaceCommand.h"


class ModifiersLayer : public Layer, public Observable, public Observer {
public:
    ModifiersLayer(const std::string& name, CommandRegistry& commandRegistry)
        : Layer(name), m_commandRegistry(commandRegistry) {}

    void onEvent(Event& event) override
    {
		if (event.getType() == EventType::MouseButtonPress)
		{
			if (m_isMouseInsideWindow) {
				event.isHandled = true;
			}
		}
    }

	void onImGuiRender() override {
		ImGui::Begin(this->getName().c_str());

		// Get window position and size
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 mousePos = ImGui::GetMousePos();

		m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
			mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);


        if (ImGui::Button("Solidify")){
            SolidifyMeshesCommand* solidifyMeshesCommand = m_commandRegistry.getCommand<SolidifyMeshesCommand>();
            solidifyMeshesCommand->execute(); 
        }

		ImGui::End();
	}

private:
	CommandRegistry& m_commandRegistry;
    bool m_isMouseInsideWindow;
};
