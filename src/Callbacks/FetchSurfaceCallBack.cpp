#include "FetchSurfaceCallBack.h"
#include "CallbackRegister.h"
#include "../Callables/CallableIDs.h"
namespace {
    const bool registered = []() {
        CallbackRegistry::instance().registerCallbackFactory(
            FetchSurfaceCallBack::ID,
            []() -> std::unique_ptr<CallbackConcept> {
                // create and configure a local composer, then pass it by const-ref to the callback
                auto composer = std::make_unique<FunctionComposer>();
                FunctionNode* root = composer->initRootByID(FETCHED_SURFACE_VERTEX_GEN_CALLABLE, true);
                composer->addFuncByID(root, MESH_VAO_INIT_CALLABLE);
                composer->addFuncByID(root, SCENE_MESH_ADDER_CALLABLE);
                composer->addFuncByID(root, MESH_OUTLINER_ADDER_CALLABLE);
                return std::make_unique<FetchSurfaceCallBack>(std::move(composer));
            }
        );
        return true;
    }();
}
FetchSurfaceCallBack::FetchSurfaceCallBack(std::unique_ptr<FunctionComposer> functionComposer)
		: ComposedCallback(std::move(functionComposer))
	{}