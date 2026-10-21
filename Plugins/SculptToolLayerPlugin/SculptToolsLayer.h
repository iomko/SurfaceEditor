#pragma once
#include <string>
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include "../../src/UI/LayerRegistry.h"
#include <Components/Window.h>
#include <Components/Drag.h>
#include <Components/CollapsingHeader.h>

class SculptToolsLayer : public Layer, public Observable, public Observer, public ui::components::Window {
public:
	SculptToolsLayer(const std::string& name);

	void onEvent(Event& event) override;

	void onImGuiRender() override;

protected:

    void initWindowConfig() override;

    void initComponents() override;

private:
	bool m_isMouseInsideWindow;

	float m_brushStrength;

	float m_brushRadius;

	ITool* m_brushTool;

	ui::components::CollapsingHeader* m_collapsingHeader;

	ui::components::Drag<float>* m_dragStrength;

	ui::components::Drag<float>* m_dragRadius;
};
