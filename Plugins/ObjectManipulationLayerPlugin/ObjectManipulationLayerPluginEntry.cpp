#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"

class ObjectManipulationLayerPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        setupLayerPlugin("OBJECT_MANIPULATION_LAYER", "ObjectManipulationLayer", false);
        setupLayerPlugin("OBJECTS_LAYER", "ObjectsLayer", false);
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
    return new ObjectManipulationLayerPlugin();
}