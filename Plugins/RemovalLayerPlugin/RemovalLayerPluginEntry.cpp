#include "../../src/Core/PluginAPI.h"
#include "../../src/UI/LayerIDs.h"


class RemovalLayerPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupLayerPlugin(REMOVAL_LAYER, "RemovalLayer", false);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "RemovalLayerPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new RemovalLayerPlugin();
}