#pragma once
#include <string>
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include "../../src/UI/WindowLayerBus.h"
#include "../../src/UI/LayerIDs.h"
#include "../../src/UI/LayerRegistry.h"

struct PrintableMeshSettingsPopUpLayerState : LayerState {
    float m_layerHeight = 1.0f;
    bool m_isOpen = false;
    bool m_isMouseInsideWindow;
    PrintableMesh* m_selectedPrintableMesh = nullptr;
    Mesh* m_selectedMesh = nullptr;
};


class PrintableMeshSettingsPopUpLayer : public LayerWithID<PRINTABLE_MESH_SETTINGS_POP_UP_LAYER>, public Observable, public Observer {
public:
    PrintableMeshSettingsPopUpLayer(const std::string& name, WindowLayerBus& windowLayerBus);

    void onEvent(Event& event) override;

	void onImGuiRender() override;

private:
    PrintableMeshSettingsPopUpLayerState m_state;
};
