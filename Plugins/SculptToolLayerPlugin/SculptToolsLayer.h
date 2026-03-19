#pragma once
#include <string>
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include "../../src/UI/LayerRegistry.h"

class SculptToolsLayer : public Layer, public Observable, public Observer {
public:
	SculptToolsLayer(const std::string& name);

	void onEvent(Event& event) override;

	void onImGuiRender() override;

private:

	bool m_isMouseInsideWindow;

	float m_brushStrength;
	float m_brushRadius;
};
