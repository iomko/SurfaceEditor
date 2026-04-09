#pragma once
#include <imgui.h>
#include "ObjectManipulationLayer.h"
#include "../AdditionLayerPlugin/AdditionLayer.h"
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/VisibilityHandler.h"
#include "../src/UI/Components/IWindow.h"
#include "../src/UI/Components/WindowStyle.h"
#include "../src/UI/Components/ButtonStyle.h"

class ObjectsLayer : public Layer, public Observable, public IWindow
{
public:
    ObjectsLayer(const std::string& name);

    void setWindowSizeAndPosition() override;

    void onImGuiRender() override;

private:
    ImVec2 m_windowSize;
    ImVec2* m_windowPos;
};