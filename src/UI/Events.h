#pragma once
#include <imgui.h>
#include <ImGuizmo.h>
#include "../../include/enums/AdditionType.h"

struct ObjectsLayerState : LayerState
{
    ImVec2 windowPos;
};

struct AdditionLayerState : LayerState
{
    AdditionType additionType;
};

struct GizmoLayerState : LayerState
{
    ImGuizmo::OPERATION operationType;
};
