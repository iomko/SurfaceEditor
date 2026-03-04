#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Commands/CommandIDs.h"

class HandleGizmoPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupPlugin(HANDLE_GIZMO_COMMAND, HANDLE_GIZMO_CALLBACK);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "HandleGizmoPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new HandleGizmoPlugin();
}