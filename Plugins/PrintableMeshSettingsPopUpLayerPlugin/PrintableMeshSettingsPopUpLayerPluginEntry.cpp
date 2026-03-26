#include "../../src/Core/PluginAPI.h"


class PrintableMeshSettingsPopUpLayerPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupLayerPlugin("PRINTABLE_MESH_SETTINGS_POP_UP_LAYER", "PopUpLayer", true);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "PrintableMeshSettingsPopUpLayerPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new PrintableMeshSettingsPopUpLayerPlugin();
}