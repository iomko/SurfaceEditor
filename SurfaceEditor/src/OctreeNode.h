#ifndef OCTREENODE_H
#define OCTREENODE_H

#include <array>
#include <vector>

template<typename T>
class OctreeNode
{
public:
    AABBBoundingRegion nodeBounds;
private:
    size_t thresholdCount;
    size_t depth = 0;
    size_t maxDepth = 8;
    OctreeNode<T>* parentNode = nullptr;
    
public:
    OctreeNode<T>* childrenNodes[8] = { nullptr };
public:
    std::vector<T> nodeData;

public:
    OctreeNode(glm::vec3 minVector, glm::vec3 maxVector, size_t thresholdCount, size_t depth, OctreeNode<T>* parentNode = nullptr)
        : nodeBounds(AABBBoundingRegion(minVector, maxVector)), thresholdCount(thresholdCount), depth(depth), parentNode(parentNode)
    {}

    ~OctreeNode()
    {
        for (size_t i = 0; i < 8; ++i) {
            delete childrenNodes[i];
        }
    }

    void addData(T data, size_t threshold, const AABBBoundingRegion& dataBounds)
    {
        if(nodeBounds.intersectsAABB(dataBounds))
        {
            if(this->depth < this->maxDepth)
            {
                if (childrenNodes[0] == nullptr)
                {
                    //split nodes
                    glm::vec3 centerBoundsPosition = nodeBounds.getMin() + ((nodeBounds.getMax() - nodeBounds.getMin()) * (1.0f / 2.0f));

                    childrenNodes[0] = new OctreeNode<T>(nodeBounds.getMin(), centerBoundsPosition, threshold, this->depth + 1, this);
                    childrenNodes[1] = new OctreeNode<T>(glm::vec3(nodeBounds.getMin().x, nodeBounds.getMin().y, nodeBounds.getMax().z),
                        centerBoundsPosition, threshold, this->depth + 1, this);
                    childrenNodes[2] = new OctreeNode<T>(glm::vec3(nodeBounds.getMax().x, nodeBounds.getMin().y, nodeBounds.getMin().z),
                        centerBoundsPosition, threshold, this->depth + 1, this);
                    childrenNodes[3] = new OctreeNode<T>(glm::vec3(nodeBounds.getMax().x, nodeBounds.getMin().y, nodeBounds.getMax().z),
                        centerBoundsPosition, threshold, this->depth + 1, this);
                    childrenNodes[4] = new OctreeNode<T>(glm::vec3(nodeBounds.getMin().x, nodeBounds.getMax().y, nodeBounds.getMin().z),
                        centerBoundsPosition, threshold, this->depth + 1, this);
                    childrenNodes[5] = new OctreeNode<T>(glm::vec3(nodeBounds.getMin().x, nodeBounds.getMax().y, nodeBounds.getMax().z),
                        centerBoundsPosition, threshold, this->depth + 1, this);
                    childrenNodes[6] = new OctreeNode<T>(glm::vec3(nodeBounds.getMax().x, nodeBounds.getMax().y, nodeBounds.getMin().z),
                        centerBoundsPosition, threshold, this->depth + 1, this);
                    childrenNodes[7] = new OctreeNode<T>(nodeBounds.getMax(), centerBoundsPosition, threshold, this->depth + 1, this);

                }
                for (size_t i = 0; i < 8; ++i)
                {
                    childrenNodes[i]->addData(data, threshold, dataBounds);
                }
                
            } else
            {
                this->nodeData.push_back(data);
            }
            
        }

    }

};

#endif