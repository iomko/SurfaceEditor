#pragma once
#include <string>
#include <variant>
#include "../../include/enums/AdditionType.h"
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

class AdditionLayer : public Layer, public Observable, public Observer, public ui::IWindow
{
public:
    AdditionLayer(const std::string& name);

	void onImGuiRender() override;

protected:
    void initConnections();

    void initWindowConfig() override;

    void initComponents() override;

private:
    void initDialogButtons();

    void initMeshComponents();

    void initSurfaceComponents();

    void handleComponentsVisibility();

	void setCreateButtonAsAddSurface();

    template <typename TParams>
    void setCreateButton(const std::string& commandId);

private:
	AdditionType m_additionType;

    ui::Layout* m_meshLayout;
    ui::Layout* m_surfaceLayout;
    ui::Layout* m_dialogLayout;

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
    ui::components::InputBox<float>* m_lowerLeftLon;
	ui::components::InputBox<float>* m_lowerLeftLat;
	ui::components::InputBox<float>* m_upperRightLon;
	ui::components::InputBox<float>* m_upperRightLat;
	ui::components::InputBox<char>*  m_apiKeyBuffer;
};
