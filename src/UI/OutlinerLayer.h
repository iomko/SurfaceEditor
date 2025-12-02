
#pragma once
#include <string>
#include <typeindex>
#include "imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"
#include <map>
#include "WindowLayerBus.h"
#include "LayerRegistry.h"
#include "LayerIDs.h"

struct OutlinerNodeConcept {
    OutlinerNodeConcept(int id, const std::string& name)
        : m_id(id), m_name(name) {}
    virtual ~OutlinerNodeConcept() = default;

    int m_id;
    std::string m_name;
    std::vector<OutlinerNodeConcept*> m_childs;
};

template<typename T>
class OutlinerNode : public OutlinerNodeConcept {
public:
    OutlinerNode(int id, const std::string& name, const T& data)
        : OutlinerNodeConcept(id, name), m_data(data) {}
    T m_data;
};

struct OutlinerLayerState : LayerState {
    std::vector<OutlinerNodeConcept*> m_nodes;
    OutlinerNodeConcept* m_currentSelectedNode = nullptr;
    bool m_isMouseInsideWindow = false;
};

// tvorba uzlov (templátové funkcie, musia byť v hlavičke)
template<typename T>
OutlinerNodeConcept* createNode(OutlinerLayerState& state, int id, const std::string& name, const T& data) {
    auto* node = new OutlinerNode<T>(id, name, data);
    state.m_nodes.emplace_back(node);
    return node;
}

template<typename T>
OutlinerNodeConcept* createChildNode(OutlinerNodeConcept* parent, int id, const std::string& name, const T& data) {
    if (!parent) return nullptr;
    auto* node = new OutlinerNode<T>(id, name, data);
    parent->m_childs.emplace_back(node);
    return node;
}

class OutlinerLayer : public LayerWithID<OUTLINER_LAYER>, public Observable, public Observer
{
public:
    OutlinerLayer(const std::string &name, WindowLayerBus &windowLayerBus);

    void onEvent(Event &event) override;

    void processTree(const std::vector<OutlinerNodeConcept *> &nodes, const ImGuiTreeNodeFlags &imguiFlags, OutlinerLayerState &outlinerLayerState);

    void onImGuiRender() override;

public:
    WindowLayerBus &m_windowLayerBus;
    OutlinerLayerState m_state;
};
