#pragma once

#include "../Primitives/AABB.h"
#include <functional>
#include <map>
#include <vector>

template<typename T>
class OctreeNode
{
public:
    AABBBoundingRegion nodeBounds;
    size_t depth = 0;
    size_t maxDepth = 3;
    OctreeNode<T>* parentNode = nullptr;
    bool isLeaf = true;
    bool delProcessed = false;
    size_t dataCount = 0;
    int childIndex = 0;
    OctreeNode<T>* childrenNodes[8] = { nullptr };
    std::vector<T> nodeData;

public:
    OctreeNode(glm::vec3 minVector, glm::vec3 maxVector, size_t depth, int childIndex, OctreeNode<T>* parentNode = nullptr)
        : nodeBounds(AABBBoundingRegion(minVector, maxVector)), depth(depth), childIndex(childIndex), parentNode(parentNode)
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

    void addData(T data, const AABBBoundingRegion& dataBounds, std::map<T, std::vector<OctreeNode<T>*>>& map)
    {

        if (nodeBounds.intersectsAABB(dataBounds))
        {
            if (this->depth < this->maxDepth)
            {
                isLeaf = false;
                if (childrenNodes[0] == nullptr)
                {
                    //split nodes
                    glm::vec3 centerBoundsPosition = nodeBounds.getMinBoundsPos() + ((nodeBounds.getMaxBoundsPos() - nodeBounds.getMinBoundsPos()) * (1.0f / 2.0f));

                    childrenNodes[0] = new OctreeNode<T>(nodeBounds.getMinBoundsPos(), centerBoundsPosition, this->depth + 1, 0, this);
                    childrenNodes[1] = new OctreeNode<T>(glm::vec3(nodeBounds.getMinBoundsPos().x, nodeBounds.getMinBoundsPos().y, nodeBounds.getMaxBoundsPos().z),
                        centerBoundsPosition, this->depth + 1, 1, this);
                    childrenNodes[2] = new OctreeNode<T>(glm::vec3(nodeBounds.getMaxBoundsPos().x, nodeBounds.getMinBoundsPos().y, nodeBounds.getMinBoundsPos().z),
                        centerBoundsPosition, this->depth + 1, 2, this);
                    childrenNodes[3] = new OctreeNode<T>(glm::vec3(nodeBounds.getMaxBoundsPos().x, nodeBounds.getMinBoundsPos().y, nodeBounds.getMaxBoundsPos().z),
                        centerBoundsPosition, this->depth + 1, 3, this);
                    childrenNodes[4] = new OctreeNode<T>(glm::vec3(nodeBounds.getMinBoundsPos().x, nodeBounds.getMaxBoundsPos().y, nodeBounds.getMinBoundsPos().z),
                        centerBoundsPosition, this->depth + 1, 4, this);
                    childrenNodes[5] = new OctreeNode<T>(glm::vec3(nodeBounds.getMinBoundsPos().x, nodeBounds.getMaxBoundsPos().y, nodeBounds.getMaxBoundsPos().z),
                        centerBoundsPosition, this->depth + 1, 5, this);
                    childrenNodes[6] = new OctreeNode<T>(glm::vec3(nodeBounds.getMaxBoundsPos().x, nodeBounds.getMaxBoundsPos().y, nodeBounds.getMinBoundsPos().z),
                        centerBoundsPosition, this->depth + 1, 6, this);
                    childrenNodes[7] = new OctreeNode<T>(nodeBounds.getMaxBoundsPos(), centerBoundsPosition, this->depth + 1, 7, this);

                }
                for (size_t i = 0; i < 8; ++i)
                {
                    childrenNodes[i]->addData(data, dataBounds, map);
                }

            }
            else
            {
                this->nodeData.push_back(data);
                ++this->dataCount;
                OctreeNode<T>* parent = this->parentNode;
                while (parent != nullptr)
                {
                    ++parent->dataCount;
                    parent = parent->parentNode;
                }
                map[data].emplace_back(this);
            }

        }

    }

    template <typename QueryShape>
    void findData(
        const QueryShape& queryShape,
        const std::function<bool(const AABBBoundingRegion&, const QueryShape&)>& octreeBoundsIntersectAlg,
        std::vector<OctreeNode<T>*>& accumulatedOctreeNodes)
    {
        if (octreeBoundsIntersectAlg(nodeBounds, queryShape)) {
            if (isLeaf) {
                if (depth < maxDepth) {
                    return;
                }
                accumulatedOctreeNodes.push_back(this);
            }
            else {
                for (OctreeNode<T>* child : childrenNodes) {
                    child->findData(queryShape, octreeBoundsIntersectAlg, accumulatedOctreeNodes);
                }
            }
        }
    }

};


template <typename Octree>
class OctreeIterator {
public:
    using ValueType = typename Octree::ValueType;
    using PointerType = ValueType*;
    using ReferenceType = ValueType&;

    PointerType current;
    OctreeIterator() = default;
    OctreeIterator(PointerType node) : current(node) {}

    //PostOrder Traversal
    OctreeIterator& operator++() {
        if (current->parentNode == nullptr)
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
        return *current;
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


template<class T>
class Octree
{
public:
    std::map<T, std::vector<OctreeNode<T>*>> leafs;
    using ValueType = OctreeNode<T>;
    using Iterator = OctreeIterator<Octree<T>>;
    OctreeNode<T>* rootNode;

    Octree(glm::vec3 minVector, glm::vec3 maxVector) : rootNode(new OctreeNode<T>(minVector, maxVector, 0, 0))
    {}
    /*
    Octree(Octree<T>&& other) noexcept
        : rootNode(std::move(other.rootNode)) {}
    */

    /*
    bool operator<(const Octree<T>& other) const {
        return rootNode->nodeBounds.getMinBoundsPos() < other.rootNode->nodeBounds.getMinBoundsPos();
    }
    */

    Octree(Octree<T>&& other) noexcept
        : rootNode(nullptr), leafs(std::move(other.leafs)) {
        rootNode = other.rootNode;
        other.rootNode = nullptr;
    }

    Iterator end() {
        return Iterator(nullptr);
    }

    Iterator begin() {
        if (rootNode == nullptr) {
            return end();
        }

        OctreeNode<T>* current = rootNode;

        // Traverse to the most left leaf node
        while (!current->isLeaf) {
            current = current->childrenNodes[0];
        }

        return Iterator(current);
    }

    ~Octree()
    {
        delete rootNode;
    }

    bool isEmpty()
    {
        return rootNode->dataCount == 0 ? true : false;
    }

    void addDataToOctree(T data, const AABBBoundingRegion& dataBounds)
    {
        rootNode->addData(data, dataBounds, this->leafs);
    }

    template <typename QueryShape>
    void findMaxDepthNodes(
        const QueryShape& queryShape,
        const std::function<bool(const AABBBoundingRegion&, const QueryShape&)>& octreeBoundsIntersectAlg,
        std::vector<OctreeNode<T>*>& accumulatedOctreeNodes)
    {
        if (octreeBoundsIntersectAlg(rootNode->nodeBounds, queryShape)) {
            rootNode->findData(queryShape, octreeBoundsIntersectAlg, accumulatedOctreeNodes);
        }
    }


	template <typename QueryShape>
	void findDataInOctree(
		const QueryShape& queryShape,
		const std::function<bool(const AABBBoundingRegion&, const QueryShape&)>& octreeBoundsIntersectAlg,
		const std::function<bool(const T&, const QueryShape&)>& dataIntersectAlg)
	{
		std::vector<OctreeNode<T>*> hitOctreeLeaves;

		if (octreeBoundsIntersectAlg(rootNode->nodeBounds, queryShape)) {
			rootNode->findData(queryShape, octreeBoundsIntersectAlg, hitOctreeLeaves);
		}

		for (const auto& leaf : hitOctreeLeaves) {

			for (const auto& data : leaf->nodeData)
			{
                dataIntersectAlg(data, queryShape);
			}
		}
	}

    void removeData(T data)
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
                if (octree_node->dataCount == 0)
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
