#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Commands/CommandIDs.h"


class AddCubePlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupPlugin(ADD_CUBE_COMMAND, ADD_CUBE_CALLBACK);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "AddCubePlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new AddCubePlugin();
}