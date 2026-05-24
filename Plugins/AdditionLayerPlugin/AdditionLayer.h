#pragma once
#include <string>
#include <variant>
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/IWindow.h"

namespace ui::components
{
    class Button;

    class CheckBox;

    template<typename T>
    class InputBox;

    template<typename T>
    class Slider;
} // ui::components

enum class AdditionType
{
	NONE 	= -1,
	PLANE 	= 0,
	CUBE	= 1,
	SURFACE	= 2
};

class AdditionLayer : public Layer, public Observable, public Observer, public IWindow
{
public:
    AdditionLayer(const std::string& name);

	void onImGuiRender() override;

    void setAdditionType(const AdditionType& additionType)
    {
        m_additionType = additionType;
    }

protected:
    void initWindowConfig() override;

    void initComponents() override;

private:
    void initMeshComponents();

    void initSurfaceComponents();

    void linkInputs();

	void setCreateButtonAsAddPlane();

	void setCreateButtonAsAddCube();

	void setCreateButtonAsAddSurface();

private:
	AdditionType m_additionType;

    // Mesh components
    ui::components::InputBox<int>*   m_subdivisionInputBox;
    ui::components::InputBox<float>* m_sizeInputBox;
    ui::components::InputBox<float>* m_posXInputBox;
    ui::components::InputBox<float>* m_posYInputBox;
    ui::components::InputBox<float>* m_posZInputBox;
    ui::components::Slider<int>*     m_subdivisionSlider;
    ui::components::Slider<float>*     m_sizeSlider;
    ui::components::Button*          m_createButton;
    ui::components::CheckBox*        m_automaticCheckBox;
    

    // Surface components
    //TODO
    // static constexpr int API_BUFFER_SIZE = 256;
    // float m_lowerLeftLon{};
	// float m_lowerLeftLat{};
	// float m_upperRightLon{};
	// float m_upperRightLat{};
	// char m_apiKeyBuffer[API_BUFFER_SIZE];
};
