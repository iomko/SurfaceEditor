#pragma once
#include <string>
#include "Commands/CreatePrintCommand.h"
#include "imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"
#include "../Commands/FetchSurfaceCommand.h"
#include "OutlinerLayer.h"

struct PrintableMeshSettingsPopUpLayerState : LayerState {
    float m_layerHeight = 1.0f;
    bool m_isOpen = false;
    bool m_isMouseInsideWindow;
    PrintableMesh* m_selectedPrintableMesh = nullptr;
    Mesh* m_selectedMesh = nullptr;
};


class PrintableMeshSettingsPopUpLayer : public Layer, public Observable, public Observer {
public:
    PrintableMeshSettingsPopUpLayer(const std::string& name, CommandRegistry& commandRegistry, WindowLayerBus& windowLayerBus)
        : Layer(name), m_commandRegistry(commandRegistry) {
            windowLayerBus.on<OutlinerLayerState>([&](OutlinerLayerState& outlinerLayerState){
                if(OutlinerNode<PrintableMesh*>* node = dynamic_cast<OutlinerNode<PrintableMesh*>*>(outlinerLayerState.m_currentSelectedNode)) {
                    m_state.m_selectedPrintableMesh = node->m_data;
                    m_state.m_isOpen = true; 
                } else if(OutlinerNode<Mesh*>* node = dynamic_cast<OutlinerNode<Mesh*>*>(outlinerLayerState.m_currentSelectedNode)){
                    m_state.m_isOpen = false;
                    m_state.m_selectedMesh = node->m_data;
                } else {
                    std::cout << "NOT DYNAMIC CAST" << std::endl;
                    m_state.m_isOpen = false;
                    m_state.m_selectedPrintableMesh = nullptr;
                };
            });
        }

    void onEvent(Event& event) override
    {
		if (event.getType() == EventType::MouseButtonPress)
		{
			if (m_state.m_isMouseInsideWindow) {
				event.isHandled = true;
			}
		}
    }

	void onImGuiRender() override {

        // Get window position and size
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 mousePos = ImGui::GetMousePos();

        m_state.m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
            mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

        if(m_state.m_isOpen) {
            ImGui::Begin(this->getName().c_str());

            if (ImGui::InputFloat("LayerHeight", &m_state.m_layerHeight)) {
                // Value was changed! 
                m_state.m_selectedPrintableMesh->removeAllLevelLayers();
                CreatePrintCommand* printCommand = m_commandRegistry.getCommand<CreatePrintCommand>();
                PrintMeshSettingsParams printMeshSettingsParams;
                printMeshSettingsParams.height = m_state.m_layerHeight;
                printMeshSettingsParams.mesh = m_state.m_selectedMesh;
                printCommand->execute(printMeshSettingsParams);
            }

            ImGui::End();
        }
	}

private:
    PrintableMeshSettingsPopUpLayerState m_state;
    CommandRegistry& m_commandRegistry;
};
