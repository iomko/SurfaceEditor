#pragma once
#include <string>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "WindowLayerBus.h"

struct PrintableMeshSettingsPopUpLayerState : LayerState {
    float m_layerHeight = 1.0f;
    bool m_isOpen = false;
    bool m_isMouseInsideWindow;
    PrintableMesh* m_selectedPrintableMesh = nullptr;
    Mesh* m_selectedMesh = nullptr;
};


class PrintableMeshSettingsPopUpLayer : public Layer, public Observable, public Observer {
public:
    PrintableMeshSettingsPopUpLayer(const std::string& name, WindowLayerBus& windowLayerBus);

    void onEvent(Event& event) override;

	void onImGuiRender() override;

private:
    PrintableMeshSettingsPopUpLayerState m_state;
};
