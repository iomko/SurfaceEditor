// Octree.h

#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeNode.h"

template<typename T, size_t Threshold>
class Octree
{
//nezabudni ho dat na private
public:
    OctreeNode<T>* rootNode;

public:
    Octree(glm::vec3 minVector, glm::vec3 maxVector) : rootNode(new OctreeNode<T>(minVector, maxVector, Threshold, 0))
    {}

    ~Octree()
    {
        delete rootNode;
    }

    void addDataToOctree(T data, const AABBBoundingRegion& dataBounds)
    {
        rootNode->addData(data, Threshold, dataBounds);
    }

    std::vector<AABBBoundingRegion> getAllBoundingBoxes()
    {
        std::vector<AABBBoundingRegion> allBoundingBoxes;
        getAllBoundingBoxesHelper(rootNode, allBoundingBoxes);
        return allBoundingBoxes;
    }

private:
    void getAllBoundingBoxesHelper(OctreeNode<T>* node, std::vector<AABBBoundingRegion>& boundingBoxes)
    {
        if (node == nullptr) {
            return;
        }

    	boundingBoxes.push_back(node->nodeBounds);

        for (size_t i = 0; i < 8; ++i) {
            getAllBoundingBoxesHelper(node->childrenNodes[i], boundingBoxes);
        }
    }
};

#endif
