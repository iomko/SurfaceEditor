#include "../../src/Core/PluginAPI.h"
#include "../../src/Commands/CommandIDs.h"
#include "../../src/Callbacks/CallbackIDs.h"

class DeleteFacePlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
	    setupPlugin(DELETE_FACE_COMMAND, DELETE_FACE_CALLBACK);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "DeleteFacePlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new DeleteFacePlugin();
}