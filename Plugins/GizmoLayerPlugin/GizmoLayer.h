#pragma once
#include "ImGuizmo.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include "../../src/UI/LayerRegistry.h"
#include "../../src/UI/LayerIDs.h"

class GizmoLayer : public LayerWithID<GIZMO_LAYER>, public Observable
{
private:
    GizmoLayerParams::Type m_type = GizmoLayerParams::Type::Disable;
    GizmoParams::SelectionMode m_selectionMode = GizmoParams::SelectionMode::Mesh;

public:
    GizmoLayer(const std::string& name);

    void onImGuiRender() override;

    ImGuizmo::OPERATION operation();
};
