
#pragma once
#include <string>
#include <typeindex>
#include "imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"
#include <map>


struct OutlinerNodeConcept {
    OutlinerNodeConcept(int id, const std::string& name) {
        m_id = id;
        m_name = name;
    }
    virtual ~OutlinerNodeConcept() = default; 

    int m_id;
    std::string m_name;
    std::vector<OutlinerNodeConcept*> m_childs;
};

template<typename T>
class OutlinerNode : public OutlinerNodeConcept {
public:
    OutlinerNode(int id, const std::string& name, const T& data) : 
        OutlinerNodeConcept(id, name), m_data(data) {}

public:
    T m_data;
};

class LayerState {
};

class WindowLayerBus {
public:
    template<typename State>
    static void emit(State& state) {
        for(auto& handler : m_handlers[typeid(State)]) {
            handler(state);
        }
    }

    template<typename State> 
    static void on(std::function<void(State&)> func) {
        m_handlers[std::type_index(typeid(State))].emplace_back(
            [func](LayerState& base) {
                func(static_cast<State&>(base));
            }
        );
    }

private:
    inline static std::map<std::type_index, std::vector<std::function<void(LayerState&)>>> m_handlers; 
};


struct OutlinerLayerState : public LayerState {
    std::vector<OutlinerNodeConcept*> m_nodes;
    OutlinerNodeConcept* m_currentSelectedNode = nullptr;
    bool m_isMouseInsideWindow;
};

class OutlinerLayer : public Layer, public Observable, public Observer {
public:
    OutlinerLayer(const std::string& name, WindowLayerBus& windowLayerBus)
        : Layer(name), m_windowLayerBus(windowLayerBus) {}

    void onEvent(Event& event) override
    {
		if (event.getType() == EventType::MouseButtonPress)
		{
			if (m_state.m_isMouseInsideWindow) {
				event.isHandled = true;
			}
		}
    }

    template<typename T>
    static OutlinerNodeConcept* addNode(int id, const std::string& name, const T& data) {
        OutlinerNodeConcept* newNode = new OutlinerNode<T>(id, name, data);
        m_state.m_nodes.emplace_back(newNode);
        return newNode;
    }
    
    template<typename T>
    static OutlinerNodeConcept* addChildNode(OutlinerNodeConcept* node, int id, const std::string& name, const T& data) {
        OutlinerNodeConcept* newNode = new OutlinerNode<T>(id, name, data);
        node->m_childs.emplace_back(newNode);
        return newNode;
    }

    void processTree(const std::vector<OutlinerNodeConcept*>& nodes, const ImGuiTreeNodeFlags& imguiFlags, OutlinerLayerState& outlinerLayerState) {
        for(OutlinerNodeConcept* node : nodes) {

            bool expanded = ImGui::TreeNodeEx((void*)(intptr_t)node->m_id, imguiFlags, "%s", node->m_name.c_str());
            if (ImGui::IsItemClicked())
            {
                outlinerLayerState.m_currentSelectedNode = node;

                m_windowLayerBus.emit<OutlinerLayerState>(m_state);
                std::cout << "Selected currentTreeNode" << std::endl;
            }

            if(expanded){
                processTree(node->m_childs, imguiFlags, outlinerLayerState);
                ImGui::TreePop();
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

        ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        
        if(!m_state.m_nodes.empty()) {
            processTree(m_state.m_nodes, base_flags, m_state);
        }

		ImGui::End();

	}
public:
    WindowLayerBus& m_windowLayerBus;
    inline static OutlinerLayerState m_state;
};
