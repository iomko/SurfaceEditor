#pragma once

#include "OctreeSpatialIndex.h"

class ExtendedFace;
class Mesh;

using FaceSpatialIndex = OctreeSpatialIndex<ExtendedFace *>;
using MeshSpatialIndex = OctreeSpatialIndex<Mesh *>;
