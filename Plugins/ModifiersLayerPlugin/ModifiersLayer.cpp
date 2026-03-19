#include "ModifiersLayer.h"
#include "imgui.h"
#include "../../src/Commands/CommandRegistry.h"

static AutoRegisterLayerArgs<ModifiersLayer,std::string, std::reference_wrapper<WindowLayerBus>> reg("MODIFIERS_LAYER");

ModifiersLayer::ModifiersLayer(const std::string &name, WindowLayerBus &windowLayerBus)
    : Layer(name)
{
    windowLayerBus.on<OutlinerLayerState>([&](OutlinerLayerState &outlinerLayerState)
                                          {
                if(OutlinerNode<Mesh>* node = dynamic_cast<OutlinerNode<Mesh>*>(outlinerLayerState.m_currentSelectedNode)) {
                    m_state.m_selectedMesh = node->m_data;
                } else {
                    std::cout << "NOT DYNAMIC CAST" << std::endl;
                    m_state.m_selectedMesh = nullptr;
                }; });
}

void ModifiersLayer::onEvent(Event &event)
{
    if (event.getType() == EventType::MouseButtonPress)
    {
        if (m_state.m_isMouseInsideWindow)
        {
            event.isHandled = true;
        }
    }
}

void ModifiersLayer::onImGuiRender()
{
    ImGui::Begin(this->getName().c_str());

    // Get window position and size
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 mousePos = ImGui::GetMousePos();

    m_state.m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                                     mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);
    
    auto *solidifyMeshesCommand = CommandRegistry::instance().getCommand("SOLIDIFY_MESHES_COMMAND");
    if(solidifyMeshesCommand)
    {
        if (ImGui::Button("Solidify"))
        {
            solidifyMeshesCommand->execute();
        }
    }
    auto *createPrintCommand = CommandRegistry::instance().getCommand("CREATE_PRINT_COMMAND");
    if(createPrintCommand)
    {
        if (ImGui::Button("CreatePrint"))
        {
            if (m_state.m_selectedMesh != nullptr)
            {            
                PrintMeshSettingsParams printMeshSettingsParams;
                printMeshSettingsParams.mesh = m_state.m_selectedMesh;
                printMeshSettingsParams.height = 1.0f;
                createPrintCommand->execute(printMeshSettingsParams);
            }
        }
    }

    ImGui::End();
}