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
                auto composer = std::make_unique<FunctionComposer>();
                FunctionNode* root = composer->initRoot<CubeVertexGenCallable>();
                composer->addFunc<MeshVaoInitCallable>(root);
                composer->addFunc<SceneMeshAdderCallable>(root);
                composer->addFunc<MeshOutlinerAdderCallable>(root);

                return std::make_unique<AddCubeCallback>(std::move(composer));
            }
        );
        return true;
    }();
}
AddCubeCallback::AddCubeCallback(std::unique_ptr<FunctionComposer> functionComposer)
		: ComposedCallback(std::move(functionComposer))
	{}