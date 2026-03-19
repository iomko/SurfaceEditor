#include "../../src/Core/PluginAPI.h"


class AddPlanePlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupPlugin("ADD_PLANE_COMMAND", "ADD_PLANE_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "AddPlanePlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new AddPlanePlugin();
}