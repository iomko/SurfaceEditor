#pragma once
#include <string>
#include "imgui.h" 
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"

class RemovalLayer : public Layer, public Observable, public Observer {
public:
	RemovalLayer(const std::string& name)
		: Layer(name) {}
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

		// Update the class variable to track if the mouse is inside the window
		m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
			mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

		if (ImGui::Button("Delete Selected Faces")) {
			auto* deleteSelectedFacesCommand = CommandRegistry::instance().getCommand("DeleteSelectedFaces");
			if(deleteSelectedFacesCommand) deleteSelectedFacesCommand->execute();
		}

		if (ImGui::Button("Delete Selected Meshes")) {
			auto* deleteSelectedMeshesCommand = CommandRegistry::instance().getCommand("DeleteSelectedMeshes");
			if(deleteSelectedMeshesCommand) deleteSelectedMeshesCommand->execute();
		}

		ImGui::End();
	}

private:

	bool m_isMouseInsideWindow;
};
