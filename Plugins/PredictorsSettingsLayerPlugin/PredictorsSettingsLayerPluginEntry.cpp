#include "../../src/Core/PluginAPI.h"


class PredictorsSettingsLayerPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupLayerPlugin("PREDICTORS_SETTINGS_LAYER", "PredictorsSettingsLayer", false);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "PredictorsSettingsLayerPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new PredictorsSettingsLayerPlugin();
}