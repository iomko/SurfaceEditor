#include "../../src/Core/PluginAPI.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Commands/CommandIDs.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/Core/Layer.h"

#include "../../src/UI/LayerIDs.h"
#include "../../src/UI/LayerRegistry.h"
#include "../../src/Patterns/Observer.h"


class AddNodesToOutlinerLayerPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        Layer* outlinerLayer = LayerRegistry::instance().getLayer(OUTLINER_LAYER, std::string("OutlinerLayer"));
	
        if (outlinerLayer)
        {		
            Observer* outlinerObserver = dynamic_cast<Observer*>(outlinerLayer);
            if(outlinerObserver)
            {
                int id = TemplateOutlinerNodeAdderCallbackIDManger::instance().GetIndex<Mesh>(false);
                auto* addNewOutlinerNodeCallBackMesh = CallbackRegistry::instance().getCallback(id);
                printf("Getting template on id %d\n", id);
                if(addNewOutlinerNodeCallBackMesh)
                {
                    Observable* addNewOutlinerNodeMeshObservalbe = dynamic_cast<Observable*>(addNewOutlinerNodeCallBackMesh);
                    addNewOutlinerNodeMeshObservalbe->addObserver(outlinerObserver);
                    outlinerObserver->observe(addNewOutlinerNodeMeshObservalbe, addNewOutlinerNodeCallBackMesh);
                }
                id = TemplateOutlinerNodeAdderCallbackIDManger::instance().GetIndex<PrintableMesh>(true);
                auto* addChildOutlinerNodeCallBackPrintableMesh = CallbackRegistry::instance().getCallback(id);
                printf("Getting template on id %d\n", id);
                
                if(addChildOutlinerNodeCallBackPrintableMesh)
                {
                    Observable* addChildOutlinerNodePrintableMeshObservable = dynamic_cast<Observable*>(addChildOutlinerNodeCallBackPrintableMesh);
                    addChildOutlinerNodePrintableMeshObservable->addObserver(outlinerObserver);
                    outlinerObserver->observe(addChildOutlinerNodePrintableMeshObservable, addChildOutlinerNodeCallBackPrintableMesh);
                }
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