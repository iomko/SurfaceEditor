#pragma once
#include <vector>
#include "../../Scene/Mesh.h"

struct SelectionHolder
{
	std::vector<Mesh*> meshes;
	std::map<Mesh*, std::vector<HalfEdgeDS::Face*>> faces;
};