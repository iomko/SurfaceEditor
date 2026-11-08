#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/Core/Layer.h"

#include "../../src/UI/LayerRegistry.h"
#include "../../src/Patterns/Observer.h"


class AddNodesToOutlinerLayerPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        Layer* outlinerLayer = LayerRegistry::instance().getLayer("OUTLINER_LAYER", std::string("OutlinerLayer"));
	
        if (outlinerLayer)
        {		
            Observer* outlinerObserver = dynamic_cast<Observer*>(outlinerLayer);
            if(outlinerObserver)
            {
                auto attachCallback = [outlinerObserver](const std::string& id)
                {
                    auto* callback = CallbackRegistry::instance().getCallback(id);
                    auto* observable = dynamic_cast<Observable*>(callback);
                    if (callback != nullptr && observable != nullptr)
                    {
                        observable->addObserver(outlinerObserver);
                        outlinerObserver->observe(observable, callback);
                    }
                };

                attachCallback(TemplateOutlinerNodeAdderCallbackIDManger::instance().GetIndex<Object>(false));
                attachCallback(TemplateOutlinerNodeAdderCallbackIDManger::instance().GetIndex<Mesh>(true));
                attachCallback(TemplateOutlinerNodeAdderCallbackIDManger::instance().GetIndex<PrintableMesh>(true));
            }
        }
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "ddNodesToOutlinerLayerPlugin"; }
    int GetBuildOrderPriority() const override { return 1; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new AddNodesToOutlinerLayerPlugin();
}