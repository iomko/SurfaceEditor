#include "../../src/Core/PluginAPI.h"
#include "../../src/Commands/CommandIDs.h"
#include "../../src/Callbacks/CallbackIDs.h"

class SolidifyMeshesPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
	    setupPlugin(SOLIDIFY_MESHES_COMMAND, SOLIDIFY_MESHES_CALLBACK);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "SolidifyMeshesPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new SolidifyMeshesPlugin();
}