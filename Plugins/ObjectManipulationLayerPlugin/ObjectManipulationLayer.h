#pragma once
#include <imgui.h>
#include <ImGuizmo.h>
#include <vector>
#include <memory>
#include <stdint.h>
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/IWindow.h"

namespace ui::components
{
    class Button;
    class RadioButton;
} // ui::components

class ObjectManipulationLayer : public Layer, public Observable, public ui::IWindow
{
public:
    ObjectManipulationLayer(const std::string& name);

    void onImGuiRender() override;

protected:
    void initConnections();

    void initWindowConfig() override;

    void initComponents() override;

private:
    void resetModeState();

    void activateSelectionMode();

    void activateGizmoMode(ImGuizmo::OPERATION operationType);

private:
    std::vector<ui::components::RadioButton*> m_radioButtons;
    ui::components::RadioButton*              m_addMeshButton;
};