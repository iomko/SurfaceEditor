#pragma once
#include <imgui.h>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "LayerRegistry.h"
#include "LayerIDs.h"
#include "VisibilityHandler.h"

class ObjectsLayer : public LayerWithID<OBJECTS>, public Observable
{
public:
    ObjectsLayer(const std::string& name);

    const ImGuiWindowFlags& setWindowPosition();

    void onImGuiRender() override;

private:
    ImVec2 m_windowSize;
};