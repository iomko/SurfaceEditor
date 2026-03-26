#include "../../src/Core/PluginAPI.h"

class DeleteMeshPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
	    setupPlugin("DELETE_MESH_COMMAND", "DELETE_MESH_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "DeleteMeshPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new DeleteMeshPlugin();
}