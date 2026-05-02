#pragma once
#include <imgui.h>
#include <vector>
#include <memory>
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/OverlappingWindow.h"
#include "../src/UI/Components/RadioImageButton.h"
#include "../src/UI/Styling/Button.h"

class ObjectManipulationLayer : public Layer, public Observable, public OverlappingWindow
{
public:
    ObjectManipulationLayer(const std::string& name);

    void onImGuiRender() override;

protected:
    ui::styling::WindowConfig initWindowConfig() override;

    void initComponents() override;

private:
    void resetModeState();

private:
    std::vector<std::unique_ptr<ui::components::RadioImageButton>> m_buttons;
};