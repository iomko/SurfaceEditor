#include "AddCubeFeature.h"
#ifdef ENABLE_ADD_CUBE
#include "../Patterns/Observer.h"

#include "../Commands/AddCubeCommand.h"
#include "../Callables/CubeVertexGenCallable.h"
#include "../Callables/MeshVaoInitCallable.h"
#include "../Callables/SceneMeshAdderCallable.h"
#include "../Callables/MeshOutlinerAdderCallable.h"

CubeFeature registerAddCubeFeature(CommandRegistry* commandRegistry)
{
    FunctionComposer* addCubeComposer = new FunctionComposer();
    FunctionNode* addCubeRoot = addCubeComposer->initRoot<CubeVertexGenCallable>();
    addCubeComposer->addFunc<MeshVaoInitCallable>(addCubeRoot);
    addCubeComposer->addFunc<SceneMeshAdderCallable>(addCubeRoot);
    addCubeComposer->addFunc<MeshOutlinerAdderCallable>(addCubeRoot);
    
    AddCubeCallback* addCubeCallBack = new AddCubeCallback(*addCubeComposer);

    commandRegistry->registerCommand<AddCubeCommand>();
    auto* addCubeCommand = commandRegistry->getCommand<AddCubeCommand>();

    addCubeCommand->addObserver(addCubeCallBack);
    addCubeCallBack->observe(addCubeCommand, addCubeCallBack);
    CubeFeature feature(addCubeComposer, addCubeCallBack);
    return feature;
}
#endif