#include "ImGuiLayer.h"

#include "Application.h"
#include "ToolBarLayer.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

void ImGuiLayer::onAttach()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(Application::getInstance().getWindow().windowHandle, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiLayer::onDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::end()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(Application::getInstance().getWindow().screenWidth, Application::getInstance().getWindow().screenHeight);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiLayer::onImGuiRender()
{

	// Rendering
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

void ImGuiLayer::onUpdate()
{
	// Updating
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		if (ImGui::IsMouseHoveringRect(itemMin_ObjectMode, itemMax_ObjectMode) && ImGui::IsMouseClicked(0))
		{
			std::cout << "Object Mode clicked" << std::endl;
			// Handle Object Mode click
		}

		if (ImGui::IsMouseHoveringRect(itemMin_PolygonMode, itemMax_PolygonMode) && ImGui::IsMouseClicked(0))
		{
			std::cout << "Polygon Mode clicked" << std::endl;
			// Handle Polygon Mode click
		}

		if (ImGui::IsMouseHoveringRect(itemMin_MoveTool, itemMax_MoveTool) && ImGui::IsMouseClicked(0))
		{
			std::cout << "Move Tool clicked" << std::endl;
			// Handle Move Tool click
		}

		if (ImGui::IsMouseHoveringRect(itemMin_RotateTool, itemMax_RotateTool) && ImGui::IsMouseClicked(0))
		{
			std::cout << "Rotate Tool clicked" << std::endl;
			// Handle Rotate Tool click
		}

		if (ImGui::IsMouseHoveringRect(itemMin_ScaleTool, itemMax_ScaleTool) && ImGui::IsMouseClicked(0))
		{
			std::cout << "Scale Tool clicked" << std::endl;
			// Handle Scale Tool click
		}
	}
}





