#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"

class MoveVertexPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupPlugin("MOVE_VERTEX_COMMAND", "MOVE_VERTEX_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "MoveVertexPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new MoveVertexPlugin();
}