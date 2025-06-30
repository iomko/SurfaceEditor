#pragma once
#include <string>
#include "imgui/imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"
#include "../Commands/FetchSurfaceCommand.h"
#include "../ViewPortsController.h"
#include "../Tools/ToolRegistry.h"
#include "../Tools/BrushTool.h"


class SculptToolsLayer : public Layer, public Observable, public Observer {
public:
	SculptToolsLayer(const std::string& name)
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

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 mousePos = ImGui::GetMousePos();

		m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
			mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

		if (ImGui::TreeNode("BrushTool")) {
			if (ImGui::Button("Choose Brush Tool")) {
				std::cout << "Chosen Brush Tool" << std::endl;
				ViewPortsHolderContext::s_viewPortsController->m_currentTool = ToolRegistry::getTool<BrushTool>();
				if(ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
				{
					delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
					ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
				}
				BrushToolParams* brushToolParams = new BrushToolParams();
				brushToolParams->brushStrength = m_brushStrength;
				brushToolParams->radius = m_brushRadius;
				ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = brushToolParams;
			}

			ImGui::SliderFloat("BrushStrength", &m_brushStrength, -50.0f, 50.0f, "%.1f");
			ImGui::SliderFloat("BrushRadius", &m_brushRadius, 0.0f, 50.0f, "%.1f");

			ImGui::TreePop();
		}

		ImGui::End();
	}

private:

	bool m_isMouseInsideWindow;

	float m_brushStrength;
	float m_brushRadius;
};
