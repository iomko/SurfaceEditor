#include "AddCubeCallback.h"
#include "../../src/Callbacks/CallbackRegister.h"

#include "../../src/Callables/CallableIDs.h"

namespace {
    const bool registered = []() {
        CallbackRegistry::instance().registerCallbackFactory(
            AddCubeCallback::ID,
            []() -> std::unique_ptr<CallbackConcept> {
                // create and configure a local composer, then pass it by const-ref to the callback
                auto composer = std::make_unique<FunctionComposer>();
                FunctionNode* root = composer->initRootByID(CUBE_VERTEX_GEN_CALLABLE, true);
                composer->addFuncByID(root, MESH_VAO_INIT_CALLABLE);
                composer->addFuncByID(root, SCENE_MESH_ADDER_CALLABLE);
                composer->addFuncByID(root, MESH_OUTLINER_ADDER_CALLABLE);

                return std::make_unique<AddCubeCallback>(std::move(composer));
            }
        );
        return true;
    }();
}
AddCubeCallback::AddCubeCallback(std::unique_ptr<FunctionComposer> functionComposer)
		: ComposedCallback(std::move(functionComposer))
	{}