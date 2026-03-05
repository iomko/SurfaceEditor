#include "PrintableMeshSettingsPopUpLayer.h"
#include "imgui.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"
#include "OutlinerLayer.h"

static AutoRegisterLayerArgs<PrintableMeshSettingsPopUpLayer,std::string,std::reference_wrapper<WindowLayerBus>> reg;

PrintableMeshSettingsPopUpLayer::PrintableMeshSettingsPopUpLayer(const std::string &name, WindowLayerBus &windowLayerBus)
    : LayerWithID(name)
{
    windowLayerBus.on<OutlinerLayerState>([&](OutlinerLayerState &outlinerLayerState)
                                          {
                if(OutlinerNode<PrintableMesh>* node = dynamic_cast<OutlinerNode<PrintableMesh>*>(outlinerLayerState.m_currentSelectedNode)) {
                    m_state.m_selectedPrintableMesh = node->m_data;
                    m_state.m_isOpen = true; 
                } else if(OutlinerNode<Mesh>* node = dynamic_cast<OutlinerNode<Mesh>*>(outlinerLayerState.m_currentSelectedNode)){
                    m_state.m_isOpen = false;
                    m_state.m_selectedMesh = node->m_data;
                } else {
                    std::cout << "NOT DYNAMIC CAST" << std::endl;
                    m_state.m_isOpen = false;
                    m_state.m_selectedPrintableMesh = nullptr;
                }; });
}

void PrintableMeshSettingsPopUpLayer::onEvent(Event &event)
{
    if (event.getType() == EventType::MouseButtonPress)
    {
        if (m_state.m_isMouseInsideWindow)
        {
            event.isHandled = true;
        }
    }
}

void PrintableMeshSettingsPopUpLayer::onImGuiRender()
{

    // Get window position and size
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 mousePos = ImGui::GetMousePos();

    m_state.m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                                     mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

    if (m_state.m_isOpen)
    {
        ImGui::Begin(this->getName().c_str());

        ImGui::InputFloat("LayerHeight", &m_state.m_layerHeight);
        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            // This triggers when the user presses Enter OR when the field loses focus.
            m_state.m_selectedPrintableMesh->removeAllLevelLayers();
            auto *printCommand = CommandRegistry::instance().getCommand(CREATE_PRINT_COMMAND);
            PrintMeshSettingsParams printMeshSettingsParams;
            printMeshSettingsParams.height = m_state.m_layerHeight;
            printMeshSettingsParams.mesh = m_state.m_selectedMesh;
            if (printCommand)
                printCommand->execute(printMeshSettingsParams);
        }

        ImGui::End();
    }
}