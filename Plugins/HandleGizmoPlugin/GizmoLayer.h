#pragma once
#include "ImGuizmo.h"
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/VisibilityHandler.h"
#include "../src/UI/LayerRegistry.h"

class GizmoLayer : public Layer, public Observable
{
public:
    GizmoLayer(const std::string& name);

    void onImGuiRender() override;
};