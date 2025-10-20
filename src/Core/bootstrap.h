#pragma once
#include <vector>
#include <memory>
#include "../Commands/CommandRegistry.h"
#include "../Modules/FetchSurfaceFeature.h"
#include "../Modules/AddPlaneFeature.h"
#include "../Modules/AddCubeFeature.h"
#include "../Modules/IFeature.h"

std::vector<IFeature*> registerFeatures(CommandRegistry* commandRegistry);    
