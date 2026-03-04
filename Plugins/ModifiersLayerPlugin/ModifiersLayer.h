#pragma once
#include <string>
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include "../../src/UI/WindowLayerBus.h"
#include "../../src/UI/LayerRegistry.h"
#include "../../src/UI/LayerIDs.h"

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
