#pragma once

#include "OctreeSpatialIndex.h"

class ExtendedFace;

using FaceSpatialIndex = OctreeSpatialIndex<ExtendedFace *>;
