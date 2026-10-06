#include "../../src/Core/PluginAPI.h"

class SplitEdgePlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupPlugin("SPLIT_EDGE_COMMAND", "SPLIT_EDGE_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "SplitEdgePlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new SplitEdgePlugin();
}