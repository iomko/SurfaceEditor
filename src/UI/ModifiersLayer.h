#pragma once
#include <string>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "WindowLayerBus.h"
#include "LayerRegistry.h"
#include "LayerIDs.h"

struct ModifiersLayerState : public LayerState{
    Mesh* m_selectedMesh = nullptr;
    bool m_isMouseInsideWindow;
};

class ModifiersLayer : public LayerWithID<MODIFIERS_LAYER>, public Observable, public Observer {
public:
    ModifiersLayer(const std::string& name, WindowLayerBus& windowLayerBus);

    void onEvent(Event& event);

	void onImGuiRender();

private:
    ModifiersLayerState m_state;
};
