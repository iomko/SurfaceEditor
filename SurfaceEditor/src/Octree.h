#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeNode.h"
#include <unordered_map>
#include <map>

template <typename Octree>
class OctreeIterator {
public:
    using ValueType = typename Octree::ValueType;
    using PointerType = typename ValueType*;
    using ReferenceType = typename ValueType&;

    PointerType current;
    OctreeIterator() = default;
    OctreeIterator(PointerType node) : current(node){}

    //PostOrder Traversal
	OctreeIterator& operator++() {
        if(current->parentNode == nullptr)
        {
            current = nullptr;
            return *this;
        }

		if (current->accessNeighbourChild() != nullptr) {
			current = current->accessNeighbourChild();
			if (!current->isLeaf) {
				do {
					current = current->childrenNodes[0];
				} while (!current->isLeaf);
			}
		}
		else {
			current = current->parentNode;
		}
		
		return *this;
	}

    ReferenceType operator*() {
        return &current;
    }

    PointerType operator->() {
        return current;
    }

    bool operator==(const OctreeIterator& other) const {
        return this->current == other.current;
    }

	bool operator!=(const OctreeIterator& other) const {
        return !(*this == other);
	}

};


template<class T, size_t Threshold>
class Octree
{
public:
    std::unordered_map<T, std::vector<OctreeNode<T>*>> leafs;
    using ValueType = OctreeNode<T>;
    using OctreeIterator = OctreeIterator<Octree<T, Threshold>>;
    OctreeNode<T>* rootNode;

    Octree(glm::vec3 minVector, glm::vec3 maxVector) : rootNode(new OctreeNode<T>(minVector, maxVector, Threshold, 0, 0))
    {}

	OctreeIterator end() {
		return OctreeIterator(nullptr);
	}

    OctreeIterator begin() {
        if (rootNode == nullptr) {
            return end();
        }

        OctreeNode<T>* current = rootNode;

        // Traverse to the most left leaf node
        while (!current->isLeaf) {
            current = current->childrenNodes[0];
        }

        return OctreeIterator(current);
    }

    ~Octree()
    {
        delete rootNode;
    }

    void addDataToOctree(T data, const AABBBoundingRegion& dataBounds)
    {
        rootNode->addData(data, Threshold, dataBounds, this->leafs);
    }

    std::pair<OctreeNode<T>*,TriangleData> findDataInOctree(const Ray& ray) {
        TriangleData returnTriangleData;

        std::vector<OctreeNode<T>*> hitOctreeLiefs;
        
        if (rootNode->nodeBounds.intersectsRay(ray)) {
            rootNode->findData(ray, hitOctreeLiefs);
        } else
        {
            return std::pair(nullptr, returnTriangleData);
        }
        //pridat lambda funkciu na to aby to fungovalo univerzalnejsie, zatial necham na triangleData
        float minDistance = std::numeric_limits<float>::max();
        OctreeNode<T>* closestNode = nullptr;
        for (OctreeNode<T>* octreeLief: hitOctreeLiefs) {
            for (const auto& t : octreeLief->nodeData) {
                const TriangleData& triangleData = t;
				float distance;
				bool hit = AABBBoundingRegion::rayTriangleIntersect(ray.origin, ray.direction, triangleData.positions[0], triangleData.positions[1], triangleData.positions[2], distance);
                if (hit) {
					if (distance < minDistance) {
						minDistance = distance;
						closestNode = octreeLief;
                        returnTriangleData.positions[0] = triangleData.positions[0];
                        returnTriangleData.positions[1] = triangleData.positions[1];
                        returnTriangleData.positions[2] = triangleData.positions[2];
					}
                }
				
            } 
        }

        return std::pair(closestNode, returnTriangleData);
	}

    void removeData(OctreeNode<T>* node, T data)
    {
		auto it = leafs.find(data);
		if (it != leafs.end()) {
			for (OctreeNode<T>* octree_node : it->second) {
                octree_node->nodeData.erase(std::remove(octree_node->nodeData.begin(), octree_node->nodeData.end(), data), octree_node->nodeData.end());
                --octree_node->dataCount;
				OctreeNode<T>* parent = octree_node->parentNode;
				while (parent != nullptr)
				{
					--parent->dataCount;
					parent = parent->parentNode;
				}
                if(octree_node->dataCount == 0)
                {
                    parent = octree_node;
					while (parent->depth != 0 && parent->parentNode->dataCount == 0)
					{
                        parent = parent->parentNode;
						parent->isLeaf = true;
						parent->~OctreeNode();
					}
                }
			}
            leafs.erase(it);
		}
    }

    void cleanAndRetrieveData() {
        
    }
};

#endif
