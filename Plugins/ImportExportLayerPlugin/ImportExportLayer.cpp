#include "ImportExportLayer.h"
#include "imgui.h" 
#include "../../src/Commands/CommandRegistry.h"
#include "ImGuiFileDialog.h"

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

    std::string filePath;
    
    
    auto* importMeshesCommand = CommandRegistry::instance().getCommand("IMPORT_MESHES_COMMAND");
    if(importMeshesCommand)
    {
        if (ImGui::Button("Import"))
        {            
            importClicked = !importClicked;
            
        }
        if(importClicked)
        {
            // open
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFile", "Choose File", ".obj,.ply");

            // display
            if (ImGuiFileDialog::Instance()->Display("ChooseFile"))
            {
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    filePath = ImGuiFileDialog::Instance()->GetFilePathName();
                    importClicked = false;
                }
                ImGuiFileDialog::Instance()->Close();
            }
            if (!filePath.empty())
            {
                
                std::cout << "filePath: " << filePath << std::endl;

                ImportExportMeshesParams importExportMeshesParams;
                importExportMeshesParams.m_filePathMeshes = filePath;
                importMeshesCommand->execute(importExportMeshesParams);
                
            }
        }
    }
        
    auto* exportMeshesCommand = CommandRegistry::instance().getCommand("EXPORT_MESHES_COMMAND");
    if(exportMeshesCommand)
    {
        if (ImGui::Button("Export"))
        {
            exportClicked = !exportClicked;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Export only selected", &onlySelected);
        if (exportClicked)
        {
            // open
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFile", "Choose File", ".obj,.ply");

            // display
            if (ImGuiFileDialog::Instance()->Display("ChooseFile"))
            {
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    filePath = ImGuiFileDialog::Instance()->GetFilePathName();
                    exportClicked = false;
                }
                ImGuiFileDialog::Instance()->Close();
            }
            if (!filePath.empty())
            {
                
                std::cout << "filePath: " << filePath << std::endl;

                ImportExportMeshesParams importExportMeshesParams;
                importExportMeshesParams.m_filePathMeshes = filePath;
                importExportMeshesParams.exportOnlySelected = onlySelected;
                exportMeshesCommand->execute(importExportMeshesParams);
                
            }
        }
    }
    

    ImGui::End();
}