
#pragma once
#include <string>
#include <typeindex>
#include "imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"
#include <map>
#include "WindowLayerBus.h"


class OutlinerLayer : public Layer, public Observable, public Observer {
public:
    OutlinerLayer(const std::string& name, WindowLayerBus& windowLayerBus);

    void onEvent(Event& event) override;

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

    void processTree(const std::vector<OutlinerNodeConcept*>& nodes, const ImGuiTreeNodeFlags& imguiFlags, OutlinerLayerState& outlinerLayerState);

	void onImGuiRender() override;
public:
    WindowLayerBus& m_windowLayerBus;
    inline static OutlinerLayerState m_state;
};
