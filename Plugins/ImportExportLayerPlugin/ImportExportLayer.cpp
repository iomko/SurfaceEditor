#include "ImportExportLayer.h"
#include "imgui.h"
#include "../../src/Commands/CommandRegistry.h"
#include "ImGuiFileDialog.h"
#include <Builders/UIWindowBuilder.h>
#include <Builders/UIButtonBuilder.h>
#include <Builders/UILabelBuilder.h>
#include <Builders/UICheckBoxBuilder.h>
#include <Components/CheckBox.h>
#include <Components/Dummy.h>
#include <Components/Label.h>
#include <Components/SameLine.h>
#include <Components/Separator.h>
#include <Styling/Font.h>
#include <Utils/VisibilityHandler.h>

namespace
{
    static constexpr const char* LAYER_NAME = "IMPORT_EXPORT_LAYER";
}

static AutoRegisterLayerArgs<ImportExportLayer, std::string> reg("IMPORT_EXPORT_LAYER");

ImportExportLayer::ImportExportLayer(const std::string &name)
    : Layer(name), ui::components::Window(LAYER_NAME)
{
    VisibilityHandler::show(LAYER_NAME);
}

void ImportExportLayer::initWindowConfig()
{
    auto posConfig = WindowPosConfigBuilder()
                         .posX(0)
                         .posY(0)
                         .build();

    auto sizeConfig = WindowSizeConfigBuilder()
                          .width(0.12f)
                          .height(0.21f)
                          .minWidth(0.01f)
                          .minHeight(0.01f)
                          .build();

    auto titleBarConfig = WindowTitleBarBuilder()
                              .background(ui::styling::Color::titleBarOrange)
                              .font(ui::styling::Font::extraBold(20.0f))
                              .build();

    auto flags = ImGuiWindowFlags_NoResize;

    m_windowConfig = WindowConfigBuilder()
                         .name("Import/Export Layer")
                         .layerName(LAYER_NAME)
                         .background(ui::styling::Color::transparentGray)
                         .titleBar(titleBarConfig)
                         .pos(posConfig)
                         .size(sizeConfig)
                         .flags(flags)
                         .build();
}

void ImportExportLayer::initComponents()
{
    auto btnConfig = ButtonConfigBuilder()
                         .rounding(8.0f)
                         .background(ui::styling::Color::lightGray)
                         .font(ui::styling::Font::regular(0.6f))
                         .size(ImVec2{0.042f, 0.04f})
                         .onHoverColor(ui::styling::Color::hoverOverOrange)
                         .onClickColor(ui::styling::Color::onClickOrange)
                         .build();

    auto chckBxConfig = CheckBoxConfigBuilder()
                            .checkMarkColor(ui::styling::Color::selectedOrange)
                            .background(ui::styling::Color::lightGray)
                            .onHoverBackground(ui::styling::Color::lightGray)
                            .onActiveBackground(ui::styling::Color::lightGray)
                            .build();

    auto defaultTextConfig = LabelConfigBuilder()
                                 .font(ui::styling::Font::regular(0.02))
                                 .build();

    ImVec2 dummySize = ImVec2(0.5f, 0.02f);

    ui::components::Layout* layout = emplaceLayout();
    layout->reserveComponents(15);
    emplaceComponent<ui::components::Dummy>(layout,(this->m_windowConfig.size.width+this->m_windowConfig.pos.posX)*2,0.04f);
    emplaceComponent<ui::components::SameLine>(layout);
    m_importButton = emplaceComponent<ui::components::Button>(
        layout, "Import", btnConfig, [this](ui::components::Button*) {
            importClicked = !importClicked;
        });
    emplaceComponent<ui::components::Dummy>(layout, dummySize.x, dummySize.y);
    emplaceComponent<ui::components::Separator>(layout, 1);
    emplaceComponent<ui::components::Dummy>(layout, dummySize.x, dummySize.y);
    emplaceComponent<ui::components::Dummy>(layout,(this->m_windowConfig.size.width+this->m_windowConfig.pos.posX)*2,0.04f);
    emplaceComponent<ui::components::SameLine>(layout);
    m_exportButton = emplaceComponent<ui::components::Button>(
        layout, "Export", btnConfig, [this](ui::components::Button*) {
            exportClicked = !exportClicked;
        });
    emplaceComponent<ui::components::Dummy>(layout, dummySize.x, dummySize.y - 0.01f);
    m_exportSelectedCheckBox = emplaceComponent<ui::components::CheckBox>(
        layout, "ExportSelectedCheckBox", chckBxConfig);
    emplaceComponent<ui::components::SameLine>(layout);
    emplaceComponent<ui::components::Label<ui::styling::TextTypes::CLASSIC>>(layout, "Export only selected", defaultTextConfig);
    emplaceComponent<ui::components::Dummy>(layout, dummySize.x, dummySize.y);
    emplaceComponent<ui::components::Separator>(layout, 1);
}

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
    this->render();
    onlySelected = *m_exportSelectedCheckBox->isChecked();

    // Get window position and size
    ImVec2 windowPos = this->m_windowConfig.pos.rawPos;
    ImVec2 windowSize = this->m_windowConfig.size.rawSize;
    ImVec2 mousePos = ImGui::GetMousePos();

    // Update the class variable to check if mouse is inside the window
    m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                             mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

    isMouseOnHeader = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                        mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + (windowSize.y*0.25));

    std::string filePath;
    auto *importMeshesCommand = CommandRegistry::instance().getCommand("IMPORT_MESHES_COMMAND");
    if (importMeshesCommand)
    {
        if (importClicked)
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

    auto *exportMeshesCommand = CommandRegistry::instance().getCommand("EXPORT_MESHES_COMMAND");
    if (exportMeshesCommand)
    {
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
}
