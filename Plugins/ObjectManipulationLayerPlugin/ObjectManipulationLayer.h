#pragma once
#include <imgui.h>
#include <vector>
#include <memory>
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
    void initWindowConfig() override;

    void initComponents() override;

private:
    void resetModeState();

    void activateSelectionMode();

    void invokeObjectsLayer(ui::components::Button* button);

private:
    std::vector<ui::components::RadioButton*> m_radioButtons;
};