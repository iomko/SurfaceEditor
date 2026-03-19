#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"

class ImportMeshesPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupPlugin("IMPORT_MESHES_COMMAND", "IMPORT_MESHES_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "ImportMeshesPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new ImportMeshesPlugin();
}