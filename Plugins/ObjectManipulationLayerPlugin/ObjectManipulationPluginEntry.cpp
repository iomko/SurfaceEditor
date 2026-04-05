#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"

class ObjectManipulationPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupPlugin("OBJECT_MANIPULATION_LAYER", "ObjectManipulationLayer", false);
        setupPlugin("OBJECTS_LAYER", "ObjectsLayer", false);
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "ObjectManipulationLayerPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new ObjectManipulationPlugin();
}