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
    size_t maxDepth = 2;
public:
    OctreeNode<T>* parentNode = nullptr;
    OctreeNode<T>* rootNode = nullptr;
    bool isLeaf = true;
    int childIndex = 0;
public:
    OctreeNode<T>* childrenNodes[8] = { nullptr };
public:
    std::vector<T> nodeData;

public:
    OctreeNode(glm::vec3 minVector, glm::vec3 maxVector, size_t thresholdCount, size_t depth, int childIndex, OctreeNode<T>* parentNode = nullptr)
        : nodeBounds(AABBBoundingRegion(minVector, maxVector)), thresholdCount(thresholdCount), depth(depth), childIndex(childIndex), parentNode(parentNode)
    {}

    ~OctreeNode()
    {
        for (size_t i = 0; i < 8; ++i) {
            delete childrenNodes[i];
        }
    }

    OctreeNode<T>* accessNeighbourChild() {
        if (childIndex == 7) {
            return nullptr;
        }
        return this->parentNode->childrenNodes[childIndex + 1];
    }

	bool operator==(const OctreeNode<T>& other) {
        return this->nodeBounds == other.nodeBounds;
	}

	bool operator!=(const OctreeNode<T>& other) {
		return !(*this == other);
	}

    void addData(T data, size_t threshold, const AABBBoundingRegion& dataBounds)
    {
        /*
        //check if added data is inside the biggest aabb
        if (depth == 0) {
            
			if (dataBounds.isWithIn(dataBounds)) {
                
            }
            else {
				//collect all the data that was added so far
			    //recalculate the rootBounds
			    //start adding data
            }
            
        }
        */
 

        if(nodeBounds.intersectsAABB(dataBounds))
        {
            if(this->depth < this->maxDepth)
            {
                isLeaf = false;
                if (childrenNodes[0] == nullptr)
                {
                    //split nodes
                    glm::vec3 centerBoundsPosition = nodeBounds.getMin() + ((nodeBounds.getMax() - nodeBounds.getMin()) * (1.0f / 2.0f));

                    childrenNodes[0] = new OctreeNode<T>(nodeBounds.getMin(), centerBoundsPosition, threshold, this->depth + 1, 0, this);
                    childrenNodes[1] = new OctreeNode<T>(glm::vec3(nodeBounds.getMin().x, nodeBounds.getMin().y, nodeBounds.getMax().z),
                        centerBoundsPosition, threshold, this->depth + 1, 1, this);
                    childrenNodes[2] = new OctreeNode<T>(glm::vec3(nodeBounds.getMax().x, nodeBounds.getMin().y, nodeBounds.getMin().z),
                        centerBoundsPosition, threshold, this->depth + 1, 2, this);
                    childrenNodes[3] = new OctreeNode<T>(glm::vec3(nodeBounds.getMax().x, nodeBounds.getMin().y, nodeBounds.getMax().z),
                        centerBoundsPosition, threshold, this->depth + 1, 3, this);
                    childrenNodes[4] = new OctreeNode<T>(glm::vec3(nodeBounds.getMin().x, nodeBounds.getMax().y, nodeBounds.getMin().z),
                        centerBoundsPosition, threshold, this->depth + 1, 4, this);
                    childrenNodes[5] = new OctreeNode<T>(glm::vec3(nodeBounds.getMin().x, nodeBounds.getMax().y, nodeBounds.getMax().z),
                        centerBoundsPosition, threshold, this->depth + 1, 5, this);
                    childrenNodes[6] = new OctreeNode<T>(glm::vec3(nodeBounds.getMax().x, nodeBounds.getMax().y, nodeBounds.getMin().z),
                        centerBoundsPosition, threshold, this->depth + 1, 6, this);
                    childrenNodes[7] = new OctreeNode<T>(nodeBounds.getMax(), centerBoundsPosition, threshold, this->depth + 1, 7, this);

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

    void findData(const Ray& ray) {
        
    }

};

#endif