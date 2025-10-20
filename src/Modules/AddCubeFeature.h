#pragma once

#include "IFeature.h"
#include "../Commands/CommandRegistry.h"
#ifdef ENABLE_ADD_CUBE
#include "../Callbacks/AddCubeCallback.h"
#include "../Callables/FunctionComposer.h"
struct CubeFeature : IFeature {
    FunctionComposer* addCubeComposer;
    AddCubeCallback* addCubeCallBack;
    CubeFeature(FunctionComposer* composer, AddCubeCallback* callback)
        : addCubeComposer(composer), addCubeCallBack(callback) {}
    ~CubeFeature() override {
        delete addCubeComposer;
        delete addCubeCallBack;
    }
};
CubeFeature registerAddCubeFeature(CommandRegistry* commandRegistry);
#else
struct CubeFeature : IFeature {};
CubeFeature registerAddCubeFeature(CommandRegistry* commandRegistry) {
    return CubeFeature();
}
#endif