#pragma once
#include <imgui.h>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "LayerRegistry.h"
#include "LayerIDs.h"
#include "VisibilityHandler.h"
#include "ObjectManipulationLayer.h"
#include "AdditionLayer.h"
#include "Components/IWindow.h"
#include "Components/WindowStyle.h"
#include "Components/ButtonStyle.h"

class ObjectsLayer : public LayerWithID<OBJECTS>, public Observable, public IWindow
{
public:
    ObjectsLayer(const std::string& name);

    void setWindowSizeAndPosition() override;

    void onImGuiRender() override;

private:
    ImVec2 m_windowSize;
    ImVec2* m_windowPos;
};