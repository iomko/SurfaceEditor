#pragma once
#include <imgui.h>
#include "ObjectManipulationLayer.h"
#include "../AdditionLayerPlugin/AdditionLayer.h"
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/VisibilityHandler.h"
#include "../src/UI/OverlappingWindow.h"

class ObjectsLayer : public Layer, public Observable, public OverlappingWindow
{
public:
    ObjectsLayer(const std::string& name);

    void onImGuiRender() override;

    void setOnFinishCallback(std::function<void()> onFinishCallback);

    void setPosCallback(std::function<ImVec2()> getPosCallback);

private:
    void initButtons();

    void updatePosition();

protected:
    ui::styling::WindowConfig initWindowConfig() override;

private:
    std::function<void()>   m_onFinish;
    std::function<ImVec2()> m_getPos;

    std::vector<std::unique_ptr<ui::components::Button>> m_buttons;
};