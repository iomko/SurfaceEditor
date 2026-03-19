#include "ImportExportLayer.h"
#include "imgui.h" 
#include "../../src/Commands/CommandRegistry.h"

static AutoRegisterLayerArgs<ImportExportLayer, std::string> reg("IMPORT_EXPORT_LAYER");

ImportExportLayer::ImportExportLayer(const std::string &name)
    : Layer(name) {}

void ImportExportLayer::onEvent(Event &event) 
{
    if (event.getType() == EventType::MouseButtonPress)
    {
        if (m_isMouseInsideWindow)
        {
            event.isHandled = true;
        }
    }
}

void ImportExportLayer::onImGuiRender()
{
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
    
    
    auto* importMeshesCommand = CommandRegistry::instance().getCommand("IMPORT_MESHES_COMMAND");
    if(importMeshesCommand)
    {
        if (ImGui::Button("Import"))
        {
            /*
            // Call a function or perform actions for Import
            filePath = WindowsFileDialogs::openFile("OBJ Files\0*.obj\0All Files\0*.*\0");
            importClicked = true;
            */
        }
        if (importClicked && !filePath.empty())
        {
            /*
            std::cout << "filePath: " << filePath << std::endl;

            ImportExportMeshesParams importExportMeshesParams;
            importExportMeshesParams.m_filePathMeshes = filePath;
            importMeshesCommand->execute(importExportMeshesParams);
            */
        }
        ImGui::SameLine();
    }
        
    auto* exportMeshesCommand = CommandRegistry::instance().getCommand("EXPORT_MESHES_COMMAND");
    if(exportMeshesCommand)
    {
        if (ImGui::Button("Export"))
        {
            /*
            // Call a function or perform actions for Import
            filePath = WindowsFileDialogs::saveFile("OBJ Files\0*.obj\0All Files\0*.*\0");
            exportClicked = true;
            */
        }
        if (exportClicked && !filePath.empty())
        {
            /*
            std::cout << "filePath: " << filePath << std::endl;

            ImportExportMeshesParams importExportMeshesParams;
            importExportMeshesParams.m_filePathMeshes = filePath;
            exportMeshesCommand->execute(importExportMeshesParams);
            */
        }
    }
    

    ImGui::End();
}