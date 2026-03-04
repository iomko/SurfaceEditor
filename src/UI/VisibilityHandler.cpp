#include "VisibilityHandler.h"

void VisibilityHandler::init(LayerIDS layer)
{
    hide(layer);
    setInRange(layer);
}

bool VisibilityHandler::isVisible(LayerIDS layer)
{
    return s_layerVisibility[layer] && s_layerInRange[layer];
}