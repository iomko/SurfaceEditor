#pragma once
#include "IFeature.h"
#include "../Commands/CommandRegistry.h"
#ifdef ENABLE_FETCH_SURFACE
#include "../Callbacks/FetchSurfaceCallBack.h"
#include "../Callables/FunctionComposer.h"
struct SurfaceFeature : IFeature {
    FunctionComposer* addPlaneComposer;
    FetchSurfaceCallBack* fetchSurfaceCallBack;
    SurfaceFeature(FunctionComposer* composer, FetchSurfaceCallBack* callback)
        : addPlaneComposer(composer), fetchSurfaceCallBack(callback) {}
    ~SurfaceFeature() override{
        delete addPlaneComposer;
        delete fetchSurfaceCallBack;
    }
};
SurfaceFeature registerFetchSurfaceFeature(CommandRegistry* commandRegistry);
#else
struct SurfaceFeature : IFeature {};
SurfaceFeature registerFetchSurfaceFeature(CommandRegistry* commandRegistry) {
    return SurfaceFeature();
}
#endif