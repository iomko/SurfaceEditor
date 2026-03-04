#include "../../src/Core/PluginAPI.h"
#include "../../src/Commands/CommandIDs.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Tools/ToolIDs.h"

class DeselectFacePlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
	    setupPlugin(DESELECT_FACE_COMMAND, DESELECT_FACE_CALLBACK, FACE_DESELECTION_TOOL);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "DeselectFacePlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new DeselectFacePlugin();
}