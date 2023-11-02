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
        // Add the data to the octree
        // You can use the 'data' parameter of type T here

    }
};

#endif