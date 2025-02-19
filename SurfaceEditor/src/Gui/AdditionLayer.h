#pragma once
#include <string>
#include "imgui/imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"


class AdditionLayer : public Layer, public Observable, public Observer {
public:
    AdditionLayer(const std::string& name)
        : Layer(name)
    {
    }

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

        if (ImGui::TreeNode("Add")) {
            if (ImGui::TreeNode("Mesh")) {
                if (ImGui::TreeNode("TriangulatedPlane")) {
                    ImGui::SliderInt("Subdivision", &m_subdivision, 1, 50);
                    //ImGui::SliderFloat("Size", &m_size, 1.0f, 100.0f);
                    ImGui::SliderFloat("Size", &m_size, 1.0f, 300.0f, "%.0f");

                    if (ImGui::Button("AddToScene")) {
                        AddPlaneCommand* addPlaneCommand = CommandRegistry::getCommand<AddPlaneCommand>();

                        AddPlaneParams addPlaneCommandParams;
                        addPlaneCommandParams.m_subdivisionLevel = m_subdivision;
                        addPlaneCommandParams.m_size = m_size;
                        addPlaneCommand->execute(addPlaneCommandParams);
                    }

                    
                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        ImGui::End();
    }

private:
    bool m_isMouseInsideWindow;
    int m_subdivision = 1;
    float m_size = 1.0f;
};
