#pragma once
#include <map>
#include <string>

class VisibilityHandler
{
public:
    VisibilityHandler() = delete;

    ~VisibilityHandler() = delete;

    static void init(const std::string& layer);

    static bool isVisible(const std::string& layer);

    inline static void show(const std::string& layer)
    {
        s_layerVisibility[layer] = true;
    }

    inline static void hide(const std::string& layer)
    {
        s_layerVisibility[layer] = false;
    }

    inline static void setOutOfRange(const std::string& layer)
    {
        s_layerInRange[layer] = false;
    }

    inline static void setInRange(const std::string& layer)
    {
        s_layerInRange[layer] = true;
    }

protected:
    inline static std::map<std::string, bool> s_layerVisibility{};
    inline static std::map<std::string, bool> s_layerInRange{};
};