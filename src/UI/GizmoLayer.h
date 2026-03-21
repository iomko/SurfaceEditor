#pragma once
#include "ImGuizmo.h"
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "VisibilityHandler.h"
#include "LayerRegistry.h"
#include "LayerIDs.h"

class GizmoLayer : public LayerWithID<GIZMO_LAYER>, public Observable
{
public:
    GizmoLayer(const std::string& name);

    void onImGuiRender() override;

    inline static void init(const GizmoParams& params) { m_gizmoParams = params; }

private:
    inline static GizmoParams m_gizmoParams;
};
