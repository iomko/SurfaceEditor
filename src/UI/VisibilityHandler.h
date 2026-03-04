#pragma once
#include <map>
#include "LayerIDs.h"

class VisibilityHandler
{
public:
    VisibilityHandler() = delete;

    ~VisibilityHandler() = delete;

    static void init(LayerIDS layer);

    static bool isVisible(LayerIDS layer);

    inline static void show(LayerIDS layer) { s_layerVisibility[layer] = true; }

    inline static void hide(LayerIDS layer) { s_layerVisibility[layer] = false; }

    inline static void setOutOfRange(LayerIDS layer) { s_layerInRange[layer] = false; }

    inline static void setInRange(LayerIDS layer) { s_layerInRange[layer] = true; }

protected:
    inline static std::map<LayerIDS, bool> s_layerVisibility{};
    inline static std::map<LayerIDS, bool> s_layerInRange{};
};