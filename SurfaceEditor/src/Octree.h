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

public:
    PointerType current;
};



template<class T, size_t Threshold>
class Octree
{
//nezabudni ho dat na private
public:
    std::unordered_map<T, std::vector<OctreeNode<T>*>> leafs;
    using ValueType = OctreeNode<T>;
    using OctreeIterator = OctreeIterator<Octree<T, Threshold>>;
    OctreeNode<T>* rootNode;
public:

    Octree(glm::vec3 minVector, glm::vec3 maxVector) : rootNode(new OctreeNode<T>(minVector, maxVector, Threshold, 0, 0))
    {}

	OctreeIterator end() {
		return OctreeIterator(nullptr);
	}

    OctreeIterator begin() {
        if (rootNode == nullptr) {
            // If the tree is empty, return end iterator
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
        rootNode->addData(data, Threshold, dataBounds, this->rootNode, this->leafs);
    }

    std::vector<AABBBoundingRegion> getAllBoundingBoxes()
    {
        std::vector<AABBBoundingRegion> allBoundingBoxes;
        getAllBoundingBoxesHelper(rootNode, allBoundingBoxes);
        return allBoundingBoxes;
    }

    std::pair<OctreeNode<T>*,TriangleData> findDataInOctree(const Ray& ray) {
        TriangleData returnTriangleData;

        std::vector<OctreeNode<T>*> hitOctreeLiefs;
        
        if (rootNode->nodeBounds.intersectsRay(ray)) {
            rootNode->findData(ray, hitOctreeLiefs);
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
        
        //new implementation
        //unikatni parenti
        std::vector<OctreeNode<T>*> parents;
		// Find the key in the map
		auto it = leafs.find(data);

		// Check if the key is found
		if (it != leafs.end()) {
			// Loop through all values associated with the key
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
        /*
        OctreeIterator::operator ++
        //postOrderTraversal
        OctreeNode<T>* currentNode = rootNode;
        //takze najskor musime ist na prveho laveho leafa
        while (currentNode->isLeaf == false) {
            currentNode = currentNode->childrenNodes[0];
        }
        
        
		//ak nie je leaf tak chod na prveho childrena
		if (current->isLeaf == false) {
			current = current->childrenNodes[0];
		}
		//ak je leaf tak si musime accesnut nasledujuceho neighbour childrena
		else {
			if (current->accessNeighbourChild() == nullptr) {
				do {
					current = current->parentNode;
				} while (current->parentNode->accessNeighbourChild() == nullptr);
			}
			current = current->accessNeighbourChild();
		}
		return *this;
        */
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
