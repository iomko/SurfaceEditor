#ifndef OCTREENODE_H
#define OCTREENODE_H
#include <array>
#include <vector>

#include "TriangleData.h"

class OctreeNode
{
	int dataThreshold;
	int depth;
	std::array<OctreeNode> childrenNodes;
	std::vector<TriangleData> datas;
	
};

#endif
