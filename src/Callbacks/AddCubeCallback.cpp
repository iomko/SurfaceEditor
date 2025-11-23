#include "AddCubeCallback.h"
#include "CallbackRegister.h"
#include "../Callables/CubeVertexGenCallable.h"
#include "../Callables/MeshVaoInitCallable.h"
#include "../Callables/SceneMeshAdderCallable.h"
#include "../Callables/MeshOutlinerAdderCallable.h"
namespace {
    const bool registered = []() {
        CallbackRegistry::instance().registerCallbackFactory(
            AddCubeCallback::ID,
            []() -> std::unique_ptr<CallbackConcept> {
                // create and configure a local composer, then pass it by const-ref to the callback
                FunctionComposer* composer = new FunctionComposer();
                FunctionNode* root = composer->initRoot<CubeVertexGenCallable>();
                composer->addFunc<MeshVaoInitCallable>(root);
                composer->addFunc<SceneMeshAdderCallable>(root);
                composer->addFunc<MeshOutlinerAdderCallable>(root);

                return std::make_unique<AddCubeCallback>(composer);
            }
        );
        return true;
    }();
}
AddCubeCallback::AddCubeCallback(FunctionComposer* functionComposer)
		: ComposedCallback(functionComposer)
	{}