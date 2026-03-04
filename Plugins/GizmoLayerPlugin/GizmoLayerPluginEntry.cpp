#include "../../src/Core/PluginAPI.h"
#include "../../src/UI/LayerIDs.h"


class ImGuiLayerPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupLayerPlugin(GIZMO_LAYER, "GizmoLayer", false);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "ImGuiLayerPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new ImGuiLayerPlugin();
}