#include "../../src/Core/PluginAPI.h"

class MeshAdderCallablesPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "MeshAdderCallablesPlugin"; }
    int GetBuildOrderPriority() const override { return 1; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new MeshAdderCallablesPlugin();
}