#pragma once
#include <string>
#include "imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"


class AdditionLayer : public Layer, public Observable, public Observer {
public:
    AdditionLayer(const std::string& name)
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

		if (ImGui::TreeNode("Add")) {
			if (ImGui::TreeNode("Mesh")) {
				if (ImGui::TreeNode("TriangulatedPlane"))
				{
					ImGui::SliderInt("Subdivision", &m_subdivision, 1, 300);
					ImGui::SliderFloat("Size", &m_size, 1.0f, 1000.0f, "%.0f");

					if (ImGui::Button("AddToScene"))
					{
						auto *addPlaneCommand = CommandRegistry::instance().getCommand("AddPlane");
						if(addPlaneCommand)
						{
							PlaneParams addPlaneCommandParams;
							addPlaneCommandParams.m_subdivisionLevel = m_subdivision;
							addPlaneCommandParams.m_size = m_size;
							addPlaneCommand->execute(addPlaneCommandParams);		
						}							
					}

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Cube"))
				{
					ImGui::SliderInt("Subdivision", &m_subdivision, 1, 300);
					ImGui::SliderFloat("Size", &m_size, 1.0f, 1000.0f, "%.0f");

					ImGui::InputInt3("Position", glm::value_ptr(m_position));
					if (ImGui::Button("AddToScene"))
					{
						auto *addCubeCommand = CommandRegistry::instance().getCommand("AddCube");
						if(addCubeCommand)
						{
							CubeParams addCubeParams;
							addCubeParams.m_size = m_size;
							addCubeParams.m_subdivisionLevel = m_subdivision;
							addCubeParams.m_position = glm::vec3(m_position);
							addCubeCommand->execute(addCubeParams);
						}
					}
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Fetch Surface Data (OpenTopography)"))
			{
				ImGui::Text("Lower-left corner:");
				ImGui::InputFloat("Lon LL", &m_lowerLeftLon);
				ImGui::InputFloat("Lat LL", &m_lowerLeftLat);

				ImGui::Text("Upper-right corner:");
				ImGui::InputFloat("Lon UR", &m_upperRightLon);
				ImGui::InputFloat("Lat UR", &m_upperRightLat);

				ImGui::InputText("API Key", m_apiKeyBuffer, IM_ARRAYSIZE(m_apiKeyBuffer));

				if (ImGui::Button("Fetch and Add to Scene"))
				{

					

					auto* fetchCommand = CommandRegistry::instance().getCommand("FetchSurface");
					if(fetchCommand) 
					{
						OpenTopoParams params;
						params.m_lowerLeftLon = m_lowerLeftLon;
						params.m_lowerLeftLat = m_lowerLeftLat;
						params.m_upperRightLon = m_upperRightLon;
						params.m_upperRightLat = m_upperRightLat;
						params.m_apiKey = std::string(m_apiKeyBuffer);

						fetchCommand->execute(params);
					}
				}

				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::End();
	}

private:

	float m_lowerLeftLon = 0.0f;
	float m_lowerLeftLat = 0.0f;
	float m_upperRightLon = 0.0f;
	float m_upperRightLat = 0.0f;

	char m_apiKeyBuffer[256] = "";

    bool m_isMouseInsideWindow;
    int m_subdivision = 1;
    float m_size = 1.0f;
	glm::ivec3 m_position = {0, 0, 0};
};
