#pragma once
#include <string>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"

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
