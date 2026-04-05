#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"

class DeleteSelectedFacesPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
	    setupPlugin("DELETE_SELECTED_FACES_COMMAND", "DELETE_SELECTED_FACES_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "DeleteSelectedFacesPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new DeleteSelectedFacesPlugin();
};