#pragma once
#include <string>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"

class RemovalLayer : public Layer, public Observable, public Observer {
public:
	RemovalLayer(const std::string& name);
	void onEvent(Event& event) override;

	void onImGuiRender() override;

private:

	bool m_isMouseInsideWindow;
};
