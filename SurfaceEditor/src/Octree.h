#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeNode.h"

template <typename Octree>
class OctreeIterator {
public:
    using ValueType = typename Octree::ValueType;
    using PointerType = typename ValueType*;
    using ReferenceType = typename ValueType&;

    OctreeIterator(PointerType node) : current(node){}

    /*
    OctreeIterator& operator++() {
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
    }
    */
    //PostOrder Traversal
	OctreeIterator& operator++() {
        /*
		if (current->parentNode == nullptr) {
            if (current.childrenNodes[0] != nullptr) {
				do {
					current = current->childrenNodes[0];
				} while (!current->isLeaf);
            }
		}
        */
		if (current->parentNode == nullptr) {
			if (current->isLeaf) {
				current = nullptr;
			}
			else {
				do {
					current = current->childrenNodes[0];
				} while (!current->isLeaf);
			}
		} else {
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
				if (current->parentNode == nullptr) {
					current = nullptr;
				}
			}
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

private:
    PointerType current;
};



template<typename T, size_t Threshold>
class Octree
{
//nezabudni ho dat na private
public:
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
		return OctreeIterator(rootNode);
	}

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
