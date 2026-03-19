#include "../../src/Core/PluginAPI.h"

class BrushToolPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
	    setupPlugin("BRUSH_TOOL_COMMAND", "BRUSH_TOOL_CALLBACK", "BRUSH_TOOL");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "BrushToolPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new BrushToolPlugin();
}