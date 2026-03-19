#include "../../src/Core/PluginAPI.h"

#include "../../src/UI/LayerRegistry.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/Core/Application.h"
#include "../../src/ViewPortsController.h"

class SelectionLayerPlugin : public IPlugin
{
public:
    void OnLoad() override 
    {
        ViewPortsController * viewPortsHolder = ViewPortsHolderContext::s_viewPortsController;
	    if(viewPortsHolder == nullptr)
            throw std::runtime_error("SELECTION LAYER DIDNT LOAD ViewPortsController");
        Layer* selectionLayer = LayerRegistry::instance().getLayer("SELECTION_LAYER", std::string("SelectionLayer"));
        if (selectionLayer)
        {
            Application& app = Application::getInstance();
            app.getLayerStack().addLayer(selectionLayer);

            // SelectionLayerCallBack selectionLayerCallBack;
            auto *selectionLayerCallBack = CallbackRegistry::instance().getCallback("SELECTION_LAYER_CALLBACK");
            auto *observerSelectionLayer = dynamic_cast<Observer *>(selectionLayerCallBack);
            auto *observableSelectionLayer = dynamic_cast<Observable *>(selectionLayer);
            if (selectionLayerCallBack && observerSelectionLayer && observableSelectionLayer)
            {
                viewPortsHolder->observe(observableSelectionLayer, selectionLayerCallBack);

                observableSelectionLayer->addObserver(observerSelectionLayer);
                observerSelectionLayer->observe(observableSelectionLayer, selectionLayerCallBack);
            }
        }
    }
    void OnUnload() override 
    {
        printf("Unregistering is not implemented yet\n");
    }
    std::string GetName() const override { return "SelectionLayerPlugin"; }
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
IPlugin* CreatePlugin()
{
    return new SelectionLayerPlugin();
}