#pragma once
#include <string>
#include <variant>
#include <memory>
#include <glm/glm.hpp>
#include <Components/Window.h>
#include <Bindings/ClampedBinding.h>
#include "../../include/enums/AdditionType.h"
#include "../src/Params/OperationParams.h"
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/LayerRegistry.h"

namespace ui::components
{
    class Button;

    class CheckBox;

    template<typename T>
    class InputBox;

    template<typename T>
    class Slider;
} // ui::components

class AdditionLayer : public Layer, public Observable, public Observer, public ui::components::Window
{
public:
    AdditionLayer(const std::string& name);

	void onImGuiRender() override;

protected:
    void initConnections();

    void initWindowConfig() override;

    void initComponents() override;

    std::unique_ptr<ui::placements::Placement> createPlacement() override;

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

    ui::components::Layout* m_meshLayout;
    ui::components::Layout* m_surfaceLayout;
    ui::components::Layout* m_dialogLayout;

    // Mesh components
    ui::components::InputBox<int>* m_subdivisionInputBox;
    ui::components::Slider<int>*   m_subdivisionSlider;
    ui::components::Button*        m_createButton;
    ui::components::CheckBox*      m_automaticCheckBox;

    // Mesh values
    std::shared_ptr<ui::bindings::ClampedBinding<int>>   m_subdivision;
    std::shared_ptr<ui::bindings::ClampedBinding<float>> m_size;
    glm::vec3                                            m_position;

    // Surface values
    OpenTopoParams m_surfaceParams;
};
