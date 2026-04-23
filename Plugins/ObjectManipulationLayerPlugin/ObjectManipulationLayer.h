#pragma once
#include <imgui.h>
#include <vector>
#include <memory>
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/OverlappingWindow.h"
#include "../src/UI/Components/ImageButton.h"

class ObjectManipulationLayer : public Layer, public Observable, public OverlappingWindow
{
public:
    ObjectManipulationLayer(const std::string& name);

    void initWindowPosConfig() override;

    void initWindowSizeConfig() override;

    void initWindowConfig() override;

    void onImGuiRender() override;

private:
    void initButtons();

private:
    std::vector<std::unique_ptr<ImageButton>> m_buttons;
};