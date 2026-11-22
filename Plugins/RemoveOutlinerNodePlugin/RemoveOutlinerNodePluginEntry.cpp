#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/Core/Layer.h"
#include "../../src/UI/LayerRegistry.h"
#include "../../src/Patterns/Observer.h"

class RemoveOutlinerNodePlugin : public IPlugin
{
public:
    void OnLoad() override
    {
        Layer* outlinerLayer = LayerRegistry::instance().getLayer("OUTLINER_LAYER", std::string("OutlinerLayer"));
        auto* observer = dynamic_cast<Observer*>(outlinerLayer);
        auto* callback = CallbackRegistry::instance().getCallback("REMOVE_OUTLINER_NODE_CALLBACK");
        auto* observable = dynamic_cast<Observable*>(callback);
        if (observer != nullptr && callback != nullptr && observable != nullptr)
        {
            observable->addObserver(observer);
            observer->observe(observable, callback);
        }
    }

    void OnUnload() override
    {
        printf("Unregistering is not implemented yet\n");
    }

    std::string GetName() const override
    {
        return "RemoveOutlinerNodePlugin";
    }

    int GetBuildOrderPriority() const override
    {
        return 1;
    }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new RemoveOutlinerNodePlugin();
}
