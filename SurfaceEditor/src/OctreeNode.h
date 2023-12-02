#pragma once

#include <vector>
#include <unordered_map>

template<typename T>
class OctreeNode
{
public:
    AABBBoundingRegion nodeBounds;
    size_t thresholdCount;
    size_t depth = 0;
    size_t maxDepth = 6;
    OctreeNode<T>* parentNode = nullptr;
    bool isLeaf = true;
    bool delProcessed = false;
    size_t dataCount = 0;
    int childIndex = 0;
    OctreeNode<T>* childrenNodes[8] = { nullptr };
    std::vector<T> nodeData;

public:
    OctreeNode(glm::vec3 minVector, glm::vec3 maxVector, size_t thresholdCount, size_t depth, int childIndex, OctreeNode<T>* parentNode = nullptr)
        : nodeBounds(AABBBoundingRegion(minVector, maxVector)), thresholdCount(thresholdCount), depth(depth), childIndex(childIndex), parentNode(parentNode)
    {}

    ~OctreeNode()
    {
		if (childrenNodes[0] != nullptr)
		{
			for (size_t i = 0; i < 8; ++i) {
				delete childrenNodes[i];
				childrenNodes[i] = nullptr;
			}
		}
        this->isLeaf = true;
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

    AABBBoundingRegion getBounds()
    {
        return this->nodeBounds;
    }

    void addData(T data, size_t threshold, const AABBBoundingRegion& dataBounds, std::unordered_map<T, std::vector<OctreeNode<T>*>>& map)
    {

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
                        centerBoundsPosition, threshold, this->depth + 1, 2,this);
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
                    childrenNodes[i]->addData(data, threshold, dataBounds, map);
                }
                
            } else
            {
                this->nodeData.push_back(data);
                ++this->dataCount;
                OctreeNode<T>* parent = this->parentNode;
                while(parent != nullptr)
                {
                    ++parent->dataCount;
                    parent = parent->parentNode;
                }
                map[data].emplace_back(this);
            }
            
        }

    }

    void findData(const Ray& ray, std::vector<OctreeNode<T>*>& acumulatedOctreeNodes) {
        if(nodeBounds.intersectsRay(ray)){
            if (isLeaf) {
                if (depth < maxDepth) {
                    return;
                }
                else {
                    acumulatedOctreeNodes.push_back(this);
                }
            }
            else {
				for (OctreeNode<T>* child : childrenNodes) {
					child->findData(ray, acumulatedOctreeNodes);
				}
            }
        }
        return;
    }

};
