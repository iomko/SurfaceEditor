#pragma once
#include <map>
#include "LayerIDs.h"

class VisibilityHandler
{
public:
    VisibilityHandler() = delete;

    ~VisibilityHandler() = delete;

    inline static void show(LayerIDS layer) { m_layerVisibility[layer] = true; }

    inline static void hide(LayerIDS layer) { m_layerVisibility[layer] = false; }

    inline static bool isVisible(LayerIDS layer) { return m_layerVisibility[layer]; }

protected:
    inline static std::map<LayerIDS, bool> m_layerVisibility{};
};