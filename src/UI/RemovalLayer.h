#pragma once
#include <string>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "LayerIDs.h"
#include "LayerRegistry.h"

class RemovalLayer : public LayerWithID<REMOVAL_LAYER>, public Observable, public Observer {
public:
	RemovalLayer(const std::string& name);
	void onEvent(Event& event) override;

	void onImGuiRender() override;

private:

	bool m_isMouseInsideWindow;
};
