#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"

class MoveSelectedFacesPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupPlugin("MOVE_SELECTED_FACES_COMMAND", "MOVE_SELECTED_FACES_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "MoveSelectedFacesPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new MoveSelectedFacesPlugin();
}