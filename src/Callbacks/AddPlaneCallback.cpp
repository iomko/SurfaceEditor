#include "AddPlaneCallback.h"
#include "CallbackRegister.h"
#include "../Callables/PlaneVertexGenCallable.h"
#include "../Callables/MeshVaoInitCallable.h"
#include "../Callables/SceneMeshAdderCallable.h"
#include "../Callables/MeshOutlinerAdderCallable.h"
namespace {
    const bool registered = []() {
        CallbackRegistry::instance().registerCallbackFactory(
            AddPlaneCallback::ID,
            []() -> std::unique_ptr<CallbackConcept> {
                // create and configure a local composer, then pass it by const-ref to the callback
                FunctionComposer* composer = new FunctionComposer();
                FunctionNode* root = composer->initRoot<PlaneVertexGenCallable>();
                composer->addFunc<MeshVaoInitCallable>(root);
                composer->addFunc<SceneMeshAdderCallable>(root);
                composer->addFunc<MeshOutlinerAdderCallable>(root);

                return std::make_unique<AddPlaneCallback>(composer);
            }
        );
        return true;
    }();
}
AddPlaneCallback::AddPlaneCallback(FunctionComposer* functionComposer)
		: ComposedCallback(functionComposer)
	{}