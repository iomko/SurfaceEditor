#pragma once
#include <string>
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include "../../src/UI/LayerIDs.h"
#include "../../src/UI/LayerRegistry.h"

class RemovalLayer : public LayerWithID<REMOVAL_LAYER>, public Observable, public Observer {
public:
	RemovalLayer(const std::string& name);
	void onEvent(Event& event) override;

	void onImGuiRender() override;

private:

	bool m_isMouseInsideWindow;
};
