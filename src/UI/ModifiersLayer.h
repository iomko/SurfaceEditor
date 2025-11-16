#pragma once
#include <string>
#include "UI/OutlinerLayer.h"
#include "imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"
#include "../Core/Layer.h"

struct ModifiersLayerState : public LayerState{
    Mesh* m_selectedMesh = nullptr;
    bool m_isMouseInsideWindow;
};

class ModifiersLayer : public Layer, public Observable, public Observer {
public:
    ModifiersLayer(const std::string& name, WindowLayerBus& windowLayerBus)
        : Layer(name) {
            windowLayerBus.on<OutlinerLayerState>([&](OutlinerLayerState& outlinerLayerState){
                if(OutlinerNode<Mesh*>* node = dynamic_cast<OutlinerNode<Mesh*>*>(outlinerLayerState.m_currentSelectedNode)) {
                    m_state.m_selectedMesh = node->m_data;
                } else {
                    std::cout << "NOT DYNAMIC CAST" << std::endl;
                    m_state.m_selectedMesh = nullptr;
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
		ImGui::Begin(this->getName().c_str());

		// Get window position and size
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 mousePos = ImGui::GetMousePos();

		m_state.m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
			mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);


        if (ImGui::Button("Solidify")){
            auto* solidifyMeshesCommand = CommandRegistry::instance().getCommand(SOLIDIFY_MESHES_COMMAND);
            if(solidifyMeshesCommand) solidifyMeshesCommand->execute(); 
        }

        if (ImGui::Button("CreatePrint")){
            if(m_state.m_selectedMesh != nullptr) {
                auto* createPrintCommand = CommandRegistry::instance().getCommand(CREATE_PRINT_COMMAND);
                PrintMeshSettingsParams printMeshSettingsParams;
                printMeshSettingsParams.mesh = m_state.m_selectedMesh;
                printMeshSettingsParams.height = 1.0f;
                if(createPrintCommand) createPrintCommand->execute(printMeshSettingsParams);
            }
        }

		ImGui::End();
	}

private:
    ModifiersLayerState m_state;
};
