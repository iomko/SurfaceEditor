#pragma once
#include "ImGuizmo.h"
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "LayerRegistry.h"
#include "LayerIDs.h"

class GizmoLayer : public LayerWithID<GIZMO_LAYER>, public Observable
{
private:
    GizmoLayerParams::Type m_type = GizmoLayerParams::Type::Disable;
    glm::mat4 m_model = glm::mat4(1.0f);

public:
    GizmoLayer(const std::string& name);

    void onImGuiRender() override;

    ImGuizmo::OPERATION operation();

    void displayGizmo();
};
