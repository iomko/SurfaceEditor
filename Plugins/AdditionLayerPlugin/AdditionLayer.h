#pragma once
#include <string>
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include "../../src/UI/LayerRegistry.h"
class AdditionLayer : public Layer, public Observable, public Observer {
public:
    AdditionLayer(const std::string& name);

    void onEvent(Event& event) override;

	void onImGuiRender() override;

private:

	float m_lowerLeftLon = 0.0f;
	float m_lowerLeftLat = 0.0f;
	float m_upperRightLon = 0.0f;
	float m_upperRightLat = 0.0f;

	char m_apiKeyBuffer[256] = "";

    bool m_isMouseInsideWindow;
    int m_subdivision = 1;
    float m_size = 1.0f;
	glm::ivec3 m_position = {0, 0, 0};
};
