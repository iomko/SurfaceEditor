#pragma once
#include <string>
#include <glm/glm.hpp>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "LayerIDs.h"
#include "LayerRegistry.h"
#include "SelectionRectangle.h"
#include "../ViewPortsController.h"
#include "../Core/Input.h"
#include "../Renderer/Renderer.h"


class SelectionLayer : public LayerWithID<SELECTION_LAYER>, public Observable {
public:
    SelectionLayer(const std::string& name);

    SelectionLayerParams::SelectionMode getSelectionMode() const;

    void onImGuiRender() override;

private:
    void drawSelectionRectangle();

private:
    SelectionLayerParams::SelectionMode m_selectionMode = SelectionLayerParams::SelectionMode::Face;
    SelectionRectangle                  m_selectionRectangle;
    RectanglePos                        m_rectanglePos;
    bool                                m_eventHandled;
};
