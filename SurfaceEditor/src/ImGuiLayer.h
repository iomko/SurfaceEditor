#pragma once

#include <iostream>

#include "Layer.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "ToolBarLayer.h"

class Application;

class ImGuiLayer : public Layer
{
public:
	void onAttach() override;
	void onDetach() override;
	void onImGuiRender() override;
	void begin();
	void end();
	void onUpdate() override;
	void onEvent(Event& event) override
	{
        /*
		// Assuming the event system provides mouse click events
	    if (event.getType() == EventType::MouseButtonPress)
	    {
	        // Check if the mouse is over the ImGui window
	        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	        {
	            // ToolBarMode radio buttons
	            if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered())
	            {
	                currentToolBarMode = ToolBarMode::ObjectMode;
	                std::cout << "currentMode: " << enumToString(currentToolBarMode) << std::endl;
	                //notify();  // Notify observers about the change
	            }

	            if (ImGui::IsMouseClicked(1) && ImGui::IsItemHovered())
	            {
	                currentToolBarMode = ToolBarMode::PolygonMode;
	                std::cout << "currentMode: " << enumToString(currentToolBarMode) << std::endl;
	                //notify();  // Notify observers about the change
	            }

	            // ToolBarTool radio buttons
	            if (ImGui::IsMouseClicked(2) && ImGui::IsItemHovered())
	            {
	                currentToolBarTool = ToolBarTool::MoveTool;
	                std::cout << "currentTool: " << enumToString(currentToolBarTool) << std::endl;
	                //notify();  // Notify observers about the change
	            }

	            if (ImGui::IsMouseClicked(3) && ImGui::IsItemHovered())
	            {
	                currentToolBarTool = ToolBarTool::RotateTool;
	                std::cout << "currentTool: " << enumToString(currentToolBarTool) << std::endl;
	                //notify();  // Notify observers about the change
	            }

	            if (ImGui::IsMouseClicked(4) && ImGui::IsItemHovered())
	            {
	                currentToolBarTool = ToolBarTool::ScaleTool;
	                std::cout << "currentTool: " << enumToString(currentToolBarTool) << std::endl;
	                //notify();  // Notify observers about the change
	            }
	        }
	    }
		*/
	}

public:
	ToolBarMode currentToolBarMode = ToolBarMode::ObjectMode;
	ToolBarTool currentToolBarTool = ToolBarTool::MoveTool;

	// Declare variables to store bounding boxes
	ImVec2 itemMin_ObjectMode;
	ImVec2 itemMax_ObjectMode;

	ImVec2 itemMin_PolygonMode;
	ImVec2 itemMax_PolygonMode;

	ImVec2 itemMin_MoveTool;
	ImVec2 itemMax_MoveTool;

	ImVec2 itemMin_RotateTool;
	ImVec2 itemMax_RotateTool;

	ImVec2 itemMin_ScaleTool;
	ImVec2 itemMax_ScaleTool;
};
