#pragma once
#include <string>
#include "imgui.h" 
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"

extern char DEBUG_LAYER[];
class DebugLayer : public Layer, public Observable, public Observer {
public:
	DebugLayer(const std::string& name);

	void onEvent(Event& event) override;

	void onImGuiRender() override;

private:
    bool m_skewCheckboxState = false;
	bool m_isMouseInsideWindow;
};
