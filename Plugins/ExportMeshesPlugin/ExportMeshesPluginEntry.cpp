#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"

class ExportMeshesPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupPlugin("EXPORT_MESHES_COMMAND", "EXPORT_MESHES_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "ExportMeshesPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new ExportMeshesPlugin();
}