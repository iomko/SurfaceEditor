#include "AddPlaneFeature.h"

#ifdef ENABLE_ADD_PLANE

#include "../Patterns/Observer.h"

#include "../Commands/AddPlaneCommand.h"
#include "../Callables/PlaneVertexGenCallable.h"
#include "../Callables/MeshVaoInitCallable.h"
#include "../Callables/SceneMeshAdderCallable.h"
#include "../Callables/MeshOutlinerAdderCallable.h"

PlaneFeature registerAddPlaneFeature(CommandRegistry* commandRegistry)
{
    //--ADD_PLANE_COMPOSER--

    FunctionComposer* addPlaneComposer = new FunctionComposer();
    FunctionNode* addPlaneRoot = addPlaneComposer->initRoot<PlaneVertexGenCallable>();
    addPlaneComposer->addFunc<MeshVaoInitCallable>(addPlaneRoot);
    addPlaneComposer->addFunc<SceneMeshAdderCallable>(addPlaneRoot);
    addPlaneComposer->addFunc<MeshOutlinerAdderCallable>(addPlaneRoot);
    AddPlaneCallback* addPlaneCallBack = new AddPlaneCallback(*addPlaneComposer);

    commandRegistry->registerCommand<AddPlaneCommand>();
    AddPlaneCommand* addPlaneCommand = commandRegistry->getCommand<AddPlaneCommand>();

    addPlaneCommand->addObserver(addPlaneCallBack);
    addPlaneCallBack->observe(addPlaneCommand, addPlaneCallBack);

    PlaneFeature feature(addPlaneComposer, addPlaneCallBack);
    return feature;
}

#endif
