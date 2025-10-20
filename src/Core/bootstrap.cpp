#include "bootstrap.h"

std::vector<IFeature*> registerFeatures(CommandRegistry* commandRegistry)
{
    std::vector<IFeature*> registeredFeatures;
    #ifdef ENABLE_ADD_PLANE
        PlaneFeature* ap = new PlaneFeature(registerAddPlaneFeature(commandRegistry));
        registeredFeatures.push_back(ap);
    #endif
    #ifdef ENABLE_ADD_CUBE
        CubeFeature* ad = new CubeFeature(registerAddCubeFeature(commandRegistry));
        registeredFeatures.push_back(ad);
    #endif
    #ifdef ENABLE_FETCH_SURFACE
        SurfaceFeature* fs = new SurfaceFeature(registerFetchSurfaceFeature(commandRegistry));
        registeredFeatures.push_back(fs);
    #endif
    return registeredFeatures;
}