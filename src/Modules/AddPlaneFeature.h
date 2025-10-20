#pragma once
#include "IFeature.h"
#include "../Commands/CommandRegistry.h"
#ifdef ENABLE_ADD_PLANE
#include "../Callbacks/AddPlaneCallback.h"
#include "../Callables/FunctionComposer.h"
struct PlaneFeature : IFeature {
    FunctionComposer* addPlaneComposer;
    AddPlaneCallback* addPlaneCallBack;
    PlaneFeature(FunctionComposer* composer, AddPlaneCallback* callback)
        : addPlaneComposer(composer), addPlaneCallBack(callback) {}
    ~PlaneFeature() override{
        delete addPlaneComposer;
        delete addPlaneCallBack;
    }
};
PlaneFeature registerAddPlaneFeature(CommandRegistry* commandRegistry);
#else
struct PlaneFeature : IFeature {};
PlaneFeature registerAddPlaneFeature(CommandRegistry* commandRegistry) {
    return PlaneFeature();
}   
#endif