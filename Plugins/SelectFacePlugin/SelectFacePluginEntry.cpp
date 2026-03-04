#include "../../src/Core/PluginAPI.h"
#include "../../src/Commands/CommandIDs.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Tools/ToolIDs.h"

class SelectFacePlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
	    setupPlugin(SELECT_FACE_COMMAND, SELECT_FACE_CALLBACK, FACE_SELECTION_TOOL);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "SelectFacePlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new SelectFacePlugin();
}