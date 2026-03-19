#include "../../src/Core/PluginAPI.h"

class SelectMeshPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
	    setupPlugin("SELECT_MESH_COMMAND", "SELECT_MESH_CALLBACK", "MESH_SELECTION_TOOL");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "SelectMeshPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new SelectMeshPlugin();
}