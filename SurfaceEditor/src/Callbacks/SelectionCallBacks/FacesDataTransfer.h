#pragma once
#include <vector>
#include "../../DataStructures/HalfEdge.h"
#include "../../Mesh.h"

struct FacesDataTransfer
{
	inline static std::map<Mesh*,std::set<HalfEdgeDS::Face*>> m_transferedData;
};
