#include "FetchSurfaceFeature.h"

#ifdef ENABLE_FETCH_SURFACE

#include "../Patterns/Observer.h"

#include "../Commands/FetchSurfaceCommand.h"
#include "../Callables/FetchedSurfaceVertexGenCallable.h"
#include "../Callables/MeshVaoInitCallable.h"
#include "../Callables/SceneMeshAdderCallable.h"
#include "../Callables/MeshOutlinerAdderCallable.h"

SurfaceFeature registerFetchSurfaceFeature(CommandRegistry* commandRegistry)
{
    //--ADD_PLANE_COMPOSER--

    FunctionComposer* fetchSurfaceComposer =  new FunctionComposer();
	FunctionNode* fetchSurfaceRoot = fetchSurfaceComposer->initRoot<FetchedSurfaceVertexGenCallable>();
	fetchSurfaceComposer->addFunc<MeshVaoInitCallable>(fetchSurfaceRoot);
	fetchSurfaceComposer->addFunc<SceneMeshAdderCallable>(fetchSurfaceRoot);
    fetchSurfaceComposer->addFunc<MeshOutlinerAdderCallable>(fetchSurfaceRoot);
	FetchSurfaceCallBack* fetchSurfaceCallBack = new FetchSurfaceCallBack(*fetchSurfaceComposer);

    commandRegistry->registerCommand<FetchSurfaceCommand>();
	FetchSurfaceCommand* fetchSurfaceCommand = commandRegistry->getCommand<FetchSurfaceCommand>();

	fetchSurfaceCommand->addObserver(fetchSurfaceCallBack);
	fetchSurfaceCallBack->observe(fetchSurfaceCommand, fetchSurfaceCallBack);

    SurfaceFeature feature(fetchSurfaceComposer, fetchSurfaceCallBack);
    
    return feature;
}

#endif
