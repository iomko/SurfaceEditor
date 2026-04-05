#include "VisibilityHandler.h"

void VisibilityHandler::init(const std::string& layer)
{
    hide(layer);
    setInRange(layer);
}

bool VisibilityHandler::isVisible(const std::string& layer)
{
    return s_layerVisibility[layer] && s_layerInRange[layer];
}