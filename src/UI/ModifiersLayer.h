#pragma once
#include <string>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "WindowLayerBus.h"

struct ModifiersLayerState : public LayerState{
    Mesh* m_selectedMesh = nullptr;
    bool m_isMouseInsideWindow;
};

class ModifiersLayer : public Layer, public Observable, public Observer {
public:
    ModifiersLayer(const std::string& name, WindowLayerBus& windowLayerBus);

    void onEvent(Event& event);

	void onImGuiRender();

private:
    ModifiersLayerState m_state;
};
