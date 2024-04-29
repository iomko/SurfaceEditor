#pragma once
import LayerSystem.Layer;
#include "imgui/imgui.h"
#include "../WindowsFileDialogs.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/ImportMeshesCommand.h"
#include "../Commands/ExportMeshesCommand.h"

class ImportExportLayer : public Layer, public Observable
{
public:
    ImportExportLayer(ViewPortHolder* viewPortHolder, const std::string& name)
        : Layer(name)
    {
        addObserver(viewPortHolder);
    }


    void onImGuiRender() override {
        ImGui::Begin(this->getName().c_str());

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
            importMeshesCommand->m_filePathMeshes = filePath;
            importMeshesCommand->execute();
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
            exportMeshesCommand->m_filePathMeshes = filePath;
            exportMeshesCommand->execute();
        }

        ImGui::End();
    }
};
