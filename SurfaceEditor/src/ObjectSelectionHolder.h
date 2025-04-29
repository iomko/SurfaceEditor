#pragma once
#include <vector>
#include "DataStructures/HalfEdge.h"
#include "Mesh.h"

class ObjectSelectionHolder
{
public:
	std::vector<Mesh*> m_meshes;

	std::map<Mesh*, std::vector<HalfEdgeDS::Face*>> m_faces;
};