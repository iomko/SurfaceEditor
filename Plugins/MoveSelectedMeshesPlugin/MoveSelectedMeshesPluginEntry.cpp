#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"

class MoveSelectedMeshesPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupPlugin("MOVE_SELECTED_MESHES_COMMAND", "MOVE_SELECTED_MESHES_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "MoveSelectedMeshesPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new MoveSelectedMeshesPlugin();
}