#include "../../src/Core/PluginAPI.h"


class ImportExportLayerPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupLayerPlugin("IMPORT_EXPORT_LAYER", "ImportExportLayer", false);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "ImportExportLayerPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new ImportExportLayerPlugin();
}