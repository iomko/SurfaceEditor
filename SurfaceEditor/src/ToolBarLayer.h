#ifndef TOOLBARLAYER_H
#define TOOLBARLAYER_H
#include "Layer.h"

enum class ToolBarMode
{
	ObjectMode,
	PolygonMode,
};

enum class ToolBarTool
{
	MoveTool,
	RotateTool,
	ScaleTool,
};

inline std::string enumToString(ToolBarMode mode)
{
	switch (mode)
	{
	case ToolBarMode::ObjectMode:
		return "Object Mode";
	case ToolBarMode::PolygonMode:
		return "Polygon Mode";
	}
	return "Unknown";
}

inline std::string enumToString(ToolBarTool tool)
{
	switch (tool)
	{
	case ToolBarTool::MoveTool:
		return "Move Tool";
	case ToolBarTool::RotateTool:
		return "Rotate Tool";
	case ToolBarTool::ScaleTool:
		return "Scale Tool";
	}
	return "Unknown";
}

class ToolBarLayer : public Layer
{
	using CallbackFunction = std::function<void()>;

public:
	void onImGuiRender() override
	{
		ImGui::RadioButton("Object Mode", reinterpret_cast<int*>(&currentToolBarMode), static_cast<int>(ToolBarMode::ObjectMode));
		itemMin_ObjectMode = ImGui::GetItemRectMin();
		itemMax_ObjectMode = ImGui::GetItemRectMax();

		ImGui::SameLine();
		ImGui::RadioButton("Polygon Mode", reinterpret_cast<int*>(&currentToolBarMode), static_cast<int>(ToolBarMode::PolygonMode));
		itemMin_PolygonMode = ImGui::GetItemRectMin();
		itemMax_PolygonMode = ImGui::GetItemRectMax();

		ImGui::RadioButton("Move Tool", reinterpret_cast<int*>(&currentToolBarTool), static_cast<int>(ToolBarTool::MoveTool));
		itemMin_MoveTool = ImGui::GetItemRectMin();
		itemMax_MoveTool = ImGui::GetItemRectMax();

		ImGui::SameLine();
		ImGui::RadioButton("Rotate Tool", reinterpret_cast<int*>(&currentToolBarTool), static_cast<int>(ToolBarTool::RotateTool));
		itemMin_RotateTool = ImGui::GetItemRectMin();
		itemMax_RotateTool = ImGui::GetItemRectMax();

		ImGui::SameLine();
		ImGui::RadioButton("Scale Tool", reinterpret_cast<int*>(&currentToolBarTool), static_cast<int>(ToolBarTool::ScaleTool));
		itemMin_ScaleTool = ImGui::GetItemRectMin();
		itemMax_ScaleTool = ImGui::GetItemRectMax();
	}

	void onEvent(Event& event) override
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse)
		{
			if (ImGui::IsMouseHoveringRect(itemMin_ObjectMode, itemMax_ObjectMode) && ImGui::IsMouseClicked(0))
			{
				currentToolBarMode = ToolBarMode::ObjectMode;
				event.isHandled = true;
			}

			if (ImGui::IsMouseHoveringRect(itemMin_PolygonMode, itemMax_PolygonMode) && ImGui::IsMouseClicked(0))
			{
				currentToolBarMode = ToolBarMode::PolygonMode;
				event.isHandled = true;
			}

			if (ImGui::IsMouseHoveringRect(itemMin_MoveTool, itemMax_MoveTool) && ImGui::IsMouseClicked(0))
			{
				currentToolBarTool = ToolBarTool::MoveTool;
				event.isHandled = true;
			}

			if (ImGui::IsMouseHoveringRect(itemMin_RotateTool, itemMax_RotateTool) && ImGui::IsMouseClicked(0))
			{
				currentToolBarTool = ToolBarTool::RotateTool;
				event.isHandled = true;
			}

			if (ImGui::IsMouseHoveringRect(itemMin_ScaleTool, itemMax_ScaleTool) && ImGui::IsMouseClicked(0))
			{
				currentToolBarTool = ToolBarTool::ScaleTool;
				event.isHandled = true;
			}
		}
	}

	void addCallback(const CallbackFunction& callback) {
		callbacks.push_back(callback);
	}

	void notify()
	{
		for (const auto& callback : callbacks) {
			callback();
		}
	}

	ToolBarTool getToolBarToolState()
	{
		return this->currentToolBarTool;
	}

	ToolBarMode getToolBarModeState()
	{
		return this->currentToolBarMode;
	}

private:
	std::vector<CallbackFunction> callbacks;
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

#endif
