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

    void updatePosition(float x, float y);

    void onImGuiRender() override;

protected:
    ui::styling::WindowConfig initWindowConfig() override;

private:
    float m_posX;
    float m_posY;
};