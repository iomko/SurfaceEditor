#include "../../src/Core/PluginAPI.h"
#include "../../src/UI/LayerIDs.h"


class OutlinerLayerPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupLayerPlugin(OUTLINER_LAYER, "OutlinerLayer", true);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "OutlinerLayerPlugin"; }
    int GetBuildOrderPriority() const override { return 2; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new OutlinerLayerPlugin();
}