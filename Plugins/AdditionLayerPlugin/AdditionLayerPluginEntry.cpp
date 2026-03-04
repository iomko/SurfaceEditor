#include "../../src/Core/PluginAPI.h"
#include "../../src/UI/LayerIDs.h"


class AdditionLayerPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupLayerPlugin(ADDITION_LAYER, "AdditionLayer", false);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "AdditionLayerPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new AdditionLayerPlugin();
}