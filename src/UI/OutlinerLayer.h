
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
    OutlinerNode(int id, const std::string& name, T* data)
        : OutlinerNodeConcept(id, name), m_data(data) {}
    T* m_data;
};

struct OutlinerLayerState : LayerState {
    std::vector<OutlinerNodeConcept*> m_nodes;
    OutlinerNodeConcept* m_currentSelectedNode = nullptr;
    bool m_isMouseInsideWindow = false;
};

//callback ako parameter alebo vnorena trieda
class OutlinerLayer : public LayerWithID<OUTLINER_LAYER>, public Observable, public Observer
{
public:
    OutlinerLayer(const std::string &name, WindowLayerBus &windowLayerBus);

    void onEvent(Event &event) override;

    void processTree(const std::vector<OutlinerNodeConcept *> &nodes, const ImGuiTreeNodeFlags &imguiFlags, OutlinerLayerState &outlinerLayerState);

    void onImGuiRender() override;
    
    void editParams(OpParams& iParams) override;

public:
    WindowLayerBus &m_windowLayerBus;
    OutlinerLayerState m_state;
};
