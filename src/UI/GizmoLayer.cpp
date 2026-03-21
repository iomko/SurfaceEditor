#include "GizmoLayer.h"
#include "../ViewPortsController.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"
#include "LayerRegistry.h"

static AutoRegisterLayerArgs<GizmoLayer, std::string> reg;

GizmoLayer::GizmoLayer(const std::string &name) : LayerWithID(name) {}

void GizmoLayer::onImGuiRender()
{
    if (!VisibilityHandler::isVisible(GIZMO_LAYER))
    {
        return;
    }

    auto* command = CommandRegistry::instance().getCommand(HANDLE_GIZMO_COMMAND);
    if (command)
    {
        command->execute(m_gizmoParams);
    }
}
