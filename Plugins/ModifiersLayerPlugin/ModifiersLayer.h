#pragma once
#include <string>
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include "../../src/UI/WindowLayerBus.h"
#include "../../src/UI/LayerRegistry.h"
#include <Components/Window.h>
#include <Components/Button.h>

struct ModifiersLayerState : public LayerState{
    Mesh* m_selectedMesh = nullptr;
    bool m_isMouseInsideWindow;
};


class ModifiersLayer : public Layer, public Observable, public Observer, public ui::components::Window {
public:
    ModifiersLayer(const std::string& name, WindowLayerBus& windowLayerBus);

    void onEvent(Event& event);

	void onImGuiRender();

protected:
    void initWindowConfig() override;

    void initComponents() override;


private:
    ModifiersLayerState m_state;
    bool meshesCommandClicked = false;
    bool printCommandClicked = false;
    bool isMouseOnHeader = false;

private:
    ui::components::Button* m_solidifyButton;
    ui::components::Button* m_createPrintButton;
};
