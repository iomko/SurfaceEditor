#pragma once
#include <string>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "LayerIDs.h"
#include "LayerRegistry.h"

class SelectionLayer : public LayerWithID<SELECTION_LAYER>, public Observable {
public:
    SelectionLayer(const std::string& name);

    SelectionLayerParams::SelectionMode getSelectionMode() const;

    SelectionLayerParams::Type getType() const;

    void onImGuiRender() override;

private:
    SelectionLayerParams::SelectionMode m_selectionMode = SelectionLayerParams::SelectionMode::Face;
    SelectionLayerParams::Type m_type = SelectionLayerParams::Type::Selection;
};
