#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"

class MoveMeshPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupPlugin("MOVE_MESH_COMMAND", "MOVE_MESH_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "MoveMeshPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new MoveMeshPlugin();
}