
#pragma once
#include <string>
#include <typeindex>
#include "imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"
#include <map>
#include "WindowLayerBus.h"

#include "OutlinerModel.h"
class OutlinerLayer : public Layer, public Observable, public Observer
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
