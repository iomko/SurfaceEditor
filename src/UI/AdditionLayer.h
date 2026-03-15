#pragma once
#include <string>
#include <variant>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "LayerRegistry.h"
#include "LayerIDs.h"
#include "VisibilityHandler.h"
#include "Components/IWindow.h"
#include "Components/WindowStyle.h"
#include "Components/FontStyle.h"
#include "Components/CheckBoxStyle.h"
#include "Components/SliderStyle.h"
#include "Components/ButtonStyle.h"

using CommandParams = std::variant<PlaneParams, CubeParams>;

enum class AdditionType
{
	NONE 	= -1,
	PLANE 	= 0,
	CUBE	= 1,
	SURFACE	= 2
};

class AdditionLayer : public LayerWithID<ADDITION_LAYER>, public Observable, public Observer, public IWindow {
public:
    AdditionLayer(const std::string& name);

	inline static void setAdditionType(AdditionType additionType) { s_additionType = additionType; }

    void setWindowSizeAndPosition() override;

    // void onEvent(Event& event) override;

	void onImGuiRender() override;

private:
	void defaultSettingsWindow(CommandConcept* command, std::function<CommandParams()> paramsCallback);

	void addPlane();

	void addCube();

	void addSurface();

private:
	float m_lowerLeftLon = 0.0f;
	float m_lowerLeftLat = 0.0f;
	float m_upperRightLon = 0.0f;
	float m_upperRightLat = 0.0f;

	char m_apiKeyBuffer[256] = "";

    bool m_isMouseInsideWindow;
    int m_subdivision = 1;
    float m_size = 1.0f;
	bool m_automaticSubdivision{};
	float m_xPos{1.0f};
	float m_yPos{1.0f};
	float m_zPos{1.0f};

	static constexpr const auto inputBoxBackground = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);

	inline static AdditionType s_additionType = AdditionType::NONE;
};
