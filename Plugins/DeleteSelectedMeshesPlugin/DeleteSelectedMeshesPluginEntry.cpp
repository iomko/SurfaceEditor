#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"

class DeleteSelectedMeshesPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
	    setupPlugin("DELETE_SELECTED_MESHES_COMMAND", "DELETE_SELECTED_MESHES_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "DeleteSelectedMeshesPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new DeleteSelectedMeshesPlugin();
};