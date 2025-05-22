#pragma once
#include <vector>
#include "Mesh.h"

class ObjectSelectionHolder
{
public:
	std::vector<Mesh*> m_meshes;

	std::map<Mesh*, std::vector<HalfEdgeDS::Face*>> m_faces;
};