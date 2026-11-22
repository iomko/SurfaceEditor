#pragma once
#include <imgui.h>
#include <ImGuizmo.h>
#include <vector>
#include <memory>
#include <stdint.h>
#include <Components/Window.h>
#include <Components/RadioGroup.h>
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"

namespace ui::components
{
    class Button;
    class RadioButton;
} // ui::components

class ObjectManipulationLayer : public Layer, public Observable, public ui::components::Window
{
public:
    ObjectManipulationLayer(const std::string& name);

    void onImGuiRender() override;

protected:
    void initConnections();

    void initWindowConfig() override;

    void initComponents() override;

    std::unique_ptr<ui::placements::Placement> createPlacement() override;

private:
    void resetModeState();

    void activateSelectionMode();

    void activateGizmoMode(ImGuizmo::OPERATION operationType);

private:
    ui::components::RadioGroup   m_radioGroup;
    ui::components::RadioButton* m_addMeshButton;
};