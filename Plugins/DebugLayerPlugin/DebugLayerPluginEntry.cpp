#include "../../src/Core/PluginAPI.h"


class DebugLayerPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupLayerPlugin("DEBUG_LAYER", "DebugLayer", false);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "DebugLayerPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new DebugLayerPlugin();
}