#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"

class DeselectMeshPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
	    setupPlugin("DESELECT_MESH_COMMAND", "DESELECT_MESH_CALLBACK", "MESH_DESELECTION_TOOL");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "DeselectMeshPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new DeselectMeshPlugin();
}