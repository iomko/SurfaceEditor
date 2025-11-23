#include "FetchSurfaceCallBack.h"
#include "CallbackRegister.h"
#include "../Callables/FetchedSurfaceVertexGenCallable.h"
#include "../Callables/MeshVaoInitCallable.h"
#include "../Callables/SceneMeshAdderCallable.h"
#include "../Callables/MeshOutlinerAdderCallable.h"
namespace {
    const bool registered = []() {
        CallbackRegistry::instance().registerCallbackFactory(
            FetchSurfaceCallBack::ID,
            []() -> std::unique_ptr<CallbackConcept> {
                // create and configure a local composer, then pass it by const-ref to the callback
                FunctionComposer* composer = new FunctionComposer();
                FunctionNode* fetchSurfaceRoot = composer->initRoot<FetchedSurfaceVertexGenCallable>();
                composer->addFunc<MeshVaoInitCallable>(fetchSurfaceRoot);
                composer->addFunc<SceneMeshAdderCallable>(fetchSurfaceRoot);
                composer->addFunc<MeshOutlinerAdderCallable>(fetchSurfaceRoot);
                return std::make_unique<FetchSurfaceCallBack>(composer);
            }
        );
        return true;
    }();
}
FetchSurfaceCallBack::FetchSurfaceCallBack(FunctionComposer* functionComposer)
		: ComposedCallback(functionComposer)
	{}