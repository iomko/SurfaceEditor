#include "../../src/Core/PluginAPI.h"

class CreatePrintPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
	    setupPlugin("CREATE_PRINT_COMMAND", "CREATE_PRINT_STRUCTURE_CALLBACK");
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "CreatePrintPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new CreatePrintPlugin();
}