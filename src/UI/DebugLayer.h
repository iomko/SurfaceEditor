#pragma once
#include <string>
#include "imgui.h" 
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "LayerIDs.h"

class DebugLayer : public LayerWithID<DEBUG_LAYER>, public Observable, public Observer {
public:
	DebugLayer(const std::string& name);

	void onEvent(Event& event) override;

	void onImGuiRender() override;

private:
	void updateEdgesVaoData(Mesh* mesh);

	void updateFacesVaoData(Mesh* mesh);

	void unhighlightAllFaces(Mesh* mesh);

    bool m_skewCheckboxState = false;
	bool m_isMouseInsideWindow;
};
