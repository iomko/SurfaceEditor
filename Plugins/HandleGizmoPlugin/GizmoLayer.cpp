#include "GizmoLayer.h"
#include "../../src/ViewPortsController.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/UI/LayerRegistry.h"

static AutoRegisterLayerArgs<GizmoLayer, std::string> reg("GIZMO_LAYER");

GizmoLayer::GizmoLayer(const std::string& name) : Layer(name) {}

void GizmoLayer::onImGuiRender()
{
    if (!VisibilityHandler::isVisible("GIZMO_LAYER"))
    {
        return;
    }

    auto* command = CommandRegistry::instance().getCommand("HANDLE_GIZMO_COMMAND");
    if (command)
    {
        GizmoParams params;
        params.m_type = ImGuizmo::OPERATION::TRANSLATE; //TODO

        command->execute(params);
    }
}