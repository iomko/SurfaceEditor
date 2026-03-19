
#pragma once
#include <string>
#include <typeindex>
#include "imgui.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include <map>
#include "../../src/UI/WindowLayerBus.h"
#include "../../src/UI/LayerRegistry.h"

class OutlinerLayer : public Layer, public Observable, public Observer
{
public:
    OutlinerLayer(const std::string &name, WindowLayerBus &windowLayerBus);

    void onEvent(Event &event) override;

    void processTree(const std::vector<OutlinerNodeConcept *> &nodes, const ImGuiTreeNodeFlags &imguiFlags, OutlinerLayerState &outlinerLayerState);

    void onImGuiRender() override;
    
    void update(Observable* observable, const OpParams& iParams) override;

public:
    WindowLayerBus &m_windowLayerBus;
    OutlinerLayerState m_state;
};
