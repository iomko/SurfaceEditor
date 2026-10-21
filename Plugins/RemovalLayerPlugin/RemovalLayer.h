#pragma once
#include <string>
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include "../../src/UI/LayerRegistry.h"
#include <Components/Window.h>

class RemovalLayer : public Layer, public Observable, public Observer, public ui::components::Window {
public:
	RemovalLayer(const std::string& name);
	void onEvent(Event& event) override;

	void onImGuiRender() override;

protected:
    void initWindowConfig() override;

    void initComponents() override;

private:

	bool m_isMouseInsideWindow;
	bool deleteSelectedFaces = false;
	bool deleteSelectedMeshes = false;
	bool isMouseOnHeader = false;
};
