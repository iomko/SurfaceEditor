#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"

class FetchSurfacePlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupPlugin("FETCH_SURFACE_COMMAND", "FETCH_SURFACE_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "FetchSurfacePlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new FetchSurfacePlugin();
}