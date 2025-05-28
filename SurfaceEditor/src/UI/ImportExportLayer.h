#pragma once
#include "../Core/Layer.h"
#include "imgui/imgui.h"
#include "../IO/WindowsFileDialogs.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/ImportMeshesCommand.h"
#include "../Commands/ExportMeshesCommand.h"

class ImportExportLayer : public Layer, public Observable
{
public:
    ImportExportLayer(const std::string& name)
        : Layer(name)
    {}

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

		// Update the class variable to check if mouse is inside the window
		m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
			mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

        bool exportClicked = false;
        bool importClicked = false;
        std::string filePath;
        
        if (ImGui::Button("Import")) {
            // Call a function or perform actions for Import
            filePath = WindowsFileDialogs::openFile("OBJ Files\0*.obj\0All Files\0*.*\0");
            importClicked = true;
        }
        if (importClicked && !filePath.empty()) {
            std::cout << "filePath: " << filePath << std::endl;

            ImportMeshesCommand* importMeshesCommand = CommandRegistry::getCommand<ImportMeshesCommand>();

            ImportExportMeshesParams importExportMeshesParams;
            importExportMeshesParams.m_filePathMeshes = filePath;
            importMeshesCommand->execute(importExportMeshesParams);
        }

        ImGui::SameLine();

        if (ImGui::Button("Export")) {
            // Call a function or perform actions for Import
            filePath = WindowsFileDialogs::saveFile("OBJ Files\0*.obj\0All Files\0*.*\0");
            exportClicked = true;
        }
        if (exportClicked && !filePath.empty()) {
            std::cout << "filePath: " << filePath << std::endl;

            ExportMeshesCommand* exportMeshesCommand = CommandRegistry::getCommand<ExportMeshesCommand>();

            ImportExportMeshesParams importExportMeshesParams;
            importExportMeshesParams.m_filePathMeshes = filePath;
            exportMeshesCommand->execute(importExportMeshesParams);
        }

        ImGui::End();
    }

private:
    bool m_isMouseInsideWindow = false;
};
