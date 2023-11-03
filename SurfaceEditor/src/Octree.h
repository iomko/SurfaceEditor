#ifndef OCTREE_H
#define OCTREE_H
#include "OctreeNode.h"
#include "TriangleData.h"

class Octree
{
private:
    OctreeNode* rootNode;

public:
    Octree()
    {
        rootNode = nullptr;
    }

    void addData(const TriangleData& data)
    {
        

    }
};

#endif