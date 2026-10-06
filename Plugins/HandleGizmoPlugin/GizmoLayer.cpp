#include "GizmoLayer.h"
#include "../../src/ViewPortsController.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/UI/LayerRegistry.h"
#include "../../src/UI/WindowLayerBus.h"
#include "../../src/UI/Events.h"
#include <Utils/VisibilityHandler.h>

static AutoRegisterLayerArgs<GizmoLayer, std::string> reg("GIZMO_LAYER");

GizmoLayer::GizmoLayer(const std::string& name)
    : Layer(name)
{
    initConnections();
}

void GizmoLayer::initConnections()
{
    WindowLayerBus::on<GizmoLayerState>([this](GizmoLayerState& state) {
        m_currentOperation = state.operationType;
    });
}

void GizmoLayer::onImGuiRender()
{
    if (!VisibilityHandler::isVisible("GIZMO_LAYER"))
    {
        return;
    }

    static auto* command = CommandRegistry::instance().getCommand("HANDLE_GIZMO_COMMAND");
    if (command)
    {
        GizmoParams params;
        params.m_type = m_currentOperation;

        command->execute(params);
    }
}