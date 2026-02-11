#pragma once
#include <string>
#include "imgui.h" 
#include "../Patterns/Observer.h"
#include "LayerIDs.h"

class DebugLayer : public LayerWithID<DEBUG_LAYER>, public Observable, public Observer {
public:
	DebugLayer(const std::string& name);

	void onEvent(Event& event) override;

	void onImGuiRender() override;

private:
    bool m_skewCheckboxState = false;
	bool m_isMouseInsideWindow;
};
