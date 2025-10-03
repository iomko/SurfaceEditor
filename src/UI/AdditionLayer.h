#pragma once
#include <string>
#include "imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"
#include "../Commands/FetchSurfaceCommand.h"


class AdditionLayer : public Layer, public Observable, public Observer {
public:
    AdditionLayer(const std::string& name, CommandRegistry& commandRegistry)
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

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 mousePos = ImGui::GetMousePos();

		m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
			mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

		if (ImGui::TreeNode("Add")) {
			if (ImGui::TreeNode("Mesh")) {
				if (ImGui::TreeNode("TriangulatedPlane")) {
					ImGui::SliderInt("Subdivision", &m_subdivision, 1, 300);
					ImGui::SliderFloat("Size", &m_size, 1.0f, 1000.0f, "%.0f");

					if (ImGui::Button("AddToScene")) {
						AddPlaneCommand* addPlaneCommand = m_commandRegistry.getCommand<AddPlaneCommand>();

						PlaneParams addPlaneCommandParams;
						addPlaneCommandParams.m_subdivisionLevel = m_subdivision;
						addPlaneCommandParams.m_size = m_size;
						addPlaneCommand->execute(addPlaneCommandParams);
					}

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}


			if (ImGui::TreeNode("Fetch Surface Data (OpenTopography)")) {

	
				ImGui::Text("Lower-left corner:");
				ImGui::InputFloat("Lon LL", &m_lowerLeftLon);
				ImGui::InputFloat("Lat LL", &m_lowerLeftLat);

				ImGui::Text("Upper-right corner:");
				ImGui::InputFloat("Lon UR", &m_upperRightLon);
				ImGui::InputFloat("Lat UR", &m_upperRightLat);

		
				ImGui::InputText("API Key", m_apiKeyBuffer, IM_ARRAYSIZE(m_apiKeyBuffer));

			
				if (ImGui::Button("Fetch and Add to Scene")) {

					OpenTopoParams params;
					params.m_lowerLeftLon = m_lowerLeftLon;
					params.m_lowerLeftLat = m_lowerLeftLat;
					params.m_upperRightLon = m_upperRightLon;
					params.m_upperRightLat = m_upperRightLat;
					params.m_apiKey = std::string(m_apiKeyBuffer);

				
					FetchSurfaceCommand* fetchCommand = m_commandRegistry.getCommand<FetchSurfaceCommand>();
					fetchCommand->execute(params);
				}

				ImGui::TreePop();
			}
			

			ImGui::TreePop();
		}

		ImGui::End();
	}

private:
	CommandRegistry& m_commandRegistry;

	float m_lowerLeftLon = 0.0f;
	float m_lowerLeftLat = 0.0f;
	float m_upperRightLon = 0.0f;
	float m_upperRightLat = 0.0f;

	char m_apiKeyBuffer[256] = "";

    bool m_isMouseInsideWindow;
    int m_subdivision = 1;
    float m_size = 1.0f;
};
