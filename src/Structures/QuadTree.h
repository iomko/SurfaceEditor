
#pragma once

#include "Primitives/BoundingRegion2D.h"
#include "../Primitives/AABB.h"
#include <functional>
#include <map>
#include <vector>
#include <algorithm>

template<typename T>
class QuadtreeNode 
{
public:
    BoundingRegion2D nodeBounds;
    size_t depth = 0;
    size_t maxDepth = 1;
    QuadtreeNode<T>* parentNode = nullptr;
    bool isLeaf = true;
    bool delProcessed = false;
    size_t dataCount = 0;
    int childIndex = 0;
    QuadtreeNode<T>* childrenNodes[4] = { nullptr };
    std::vector<T> nodeData;

public:
    QuadtreeNode(glm::vec2 minVector, glm::vec2 maxVector, size_t depth, int childIndex, QuadtreeNode<T>* parentNode = nullptr)
        : nodeBounds(BoundingRegion2D(minVector, maxVector)), depth(depth), childIndex(childIndex), parentNode(parentNode)
    {}

    ~QuadtreeNode()
    {
        for (size_t i = 0; i < 4; ++i) {
            delete childrenNodes[i];
            childrenNodes[i] = nullptr;
        }
        this->isLeaf = true;
    }

    QuadtreeNode<T>* accessNeighbourChild() {
        if (childIndex == 3) {
            return nullptr;
        }
        return this->parentNode->childrenNodes[childIndex + 1];
    }

    bool operator==(const QuadtreeNode<T>& other) {
        return this->nodeBounds == other.nodeBounds;
    }

    bool operator!=(const QuadtreeNode<T>& other) {
        return !(*this == other);
    }

    BoundingRegion2D getBounds()
    {
        return this->nodeBounds;
    }

    void addData(T data, const BoundingRegion2D& dataBounds, std::map<T, std::vector<QuadtreeNode<T>*>>& map)
    {
        if (nodeBounds.intersectsBoundingRegion2D(dataBounds))
        {
            if (this->depth < this->maxDepth)
            {
                isLeaf = false;
                if (childrenNodes[0] == nullptr)
                {
                    //split nodes
                    glm::vec2 centerBoundsPosition = nodeBounds.getMinBoundsPos() + ((nodeBounds.getMaxBoundsPos() - nodeBounds.getMinBoundsPos()) * (1.0f / 2.0f));

                    childrenNodes[0] = new QuadtreeNode<T>(nodeBounds.getMinBoundsPos(), centerBoundsPosition, this->depth + 1, 0, this);
                    childrenNodes[1] = new QuadtreeNode<T>(glm::vec2(nodeBounds.getMaxBoundsPos().x, nodeBounds.getMinBoundsPos().y),
                        centerBoundsPosition, this->depth + 1, 1, this);
                    childrenNodes[2] = new QuadtreeNode<T>(glm::vec2(nodeBounds.getMinBoundsPos().x, nodeBounds.getMaxBoundsPos().y),
                        centerBoundsPosition, this->depth + 1, 2, this);
                    childrenNodes[3] = new QuadtreeNode<T>(nodeBounds.getMaxBoundsPos(), centerBoundsPosition, this->depth + 1, 3, this);

                }
                for (size_t i = 0; i < 4; ++i)
                {
                    childrenNodes[i]->addData(data, dataBounds, map);
                }

            }
            else
            {
                this->nodeData.push_back(data);
                ++this->dataCount;
                QuadtreeNode<T>* parent = this->parentNode;
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
        const std::function<bool(const BoundingRegion2D&, const QueryShape&)>& quadtreeBoundsIntersectAlg,
        std::vector<QuadtreeNode<T>*>& accumulatedQuadtreeNodes)
    {
        if (quadtreeBoundsIntersectAlg(nodeBounds, queryShape)) {
            if (isLeaf) {
                if (depth < maxDepth) {
                    return;
                }
                accumulatedQuadtreeNodes.push_back(this);
            }
            else {
                for (QuadtreeNode<T>* child : childrenNodes) {
                    child->findData(queryShape, quadtreeBoundsIntersectAlg, accumulatedQuadtreeNodes);
                }
            }
        }
    }

};


template <typename Quadtree>
class QuadtreeIterator {
public:
    using ValueType = typename Quadtree::ValueType;
    using PointerType = ValueType*;
    using ReferenceType = ValueType&;

    PointerType current;
    QuadtreeIterator() = default;
    QuadtreeIterator(PointerType node) : current(node) {}

    //PostOrder Traversal
    QuadtreeIterator& operator++() {
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

    bool operator==(const QuadtreeIterator& other) const {
        return this->current == other.current;
    }

    bool operator!=(const QuadtreeIterator& other) const {
        return !(*this == other);
    }

};


template<class T>
class Quadtree 
{
public:
    std::map<T, std::vector<QuadtreeNode<T>*>> leafs;
    using ValueType = QuadtreeNode<T>;
    using Iterator = QuadtreeIterator<Quadtree<T>>;
    QuadtreeNode<T>* rootNode;

    Quadtree(glm::vec2 minVector, glm::vec2 maxVector) : rootNode(new QuadtreeNode<T>(minVector, maxVector, 0, 0))
    {}

    Quadtree(Quadtree<T>&& other) noexcept
        : rootNode(nullptr), leafs(std::move(other.leafs)) {
        rootNode = other.rootNode;
        other.rootNode = nullptr;
    }

    Iterator end() {
        return Iterator(nullptr);
    }

    Iterator begin() {
        if (rootNode == nullptr || rootNode->dataCount == 0) {
            return end();
        }

        QuadtreeNode<T>* current = rootNode;

        // Traverse to the most left leaf node
        while (!current->isLeaf) {
            current = current->childrenNodes[0];
        }

        return Iterator(current);
    }

    ~Quadtree()
    {
        delete rootNode;
    }

    bool isEmpty()
    {
        return rootNode->dataCount == 0 ? true : false;
    }

    void addDataToQuadtree(T data, const BoundingRegion2D& dataBounds)
    {
        rootNode->addData(data, dataBounds, this->leafs);
    }

    template <typename QueryShape>
    void findMaxDepthNodes(
        const QueryShape& queryShape,
        const std::function<bool(const BoundingRegion2D&, const QueryShape&)>& quadtreeBoundsIntersectAlg,
        std::vector<QuadtreeNode<T>*>& accumulatedQuadtreeNodes)
    {
        if (quadtreeBoundsIntersectAlg(rootNode->nodeBounds, queryShape)) {
            rootNode->findData(queryShape, quadtreeBoundsIntersectAlg, accumulatedQuadtreeNodes);
        }
    }


	template <typename QueryShape>
	void findDataInQuadtree(
		const QueryShape& queryShape,
		const std::function<bool(const BoundingRegion2D&, const QueryShape&)>& quadtreeBoundsIntersectAlg,
		const std::function<bool(const T&, const QueryShape&)>& dataIntersectAlg)
	{
		std::vector<QuadtreeNode<T>*> hitQuadtreeLeaves;

		if (quadtreeBoundsIntersectAlg(rootNode->nodeBounds, queryShape)) {
			rootNode->findData(queryShape, quadtreeBoundsIntersectAlg, hitQuadtreeLeaves);
		}

		for (const auto& leaf : hitQuadtreeLeaves) {

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
            for (QuadtreeNode<T>* quadtree_node : it->second) {
                quadtree_node->nodeData.erase(std::remove(quadtree_node->nodeData.begin(), quadtree_node->nodeData.end(), data), quadtree_node->nodeData.end());
                --quadtree_node->dataCount;
                QuadtreeNode<T>* parent = quadtree_node->parentNode;
                while (parent != nullptr)
                {
                    --parent->dataCount;
                    parent = parent->parentNode;
                }
                if (quadtree_node->dataCount == 0)
                {
                    parent = quadtree_node;
                    while (parent->depth != 0 && parent->parentNode->dataCount == 0)
                    {
                        parent = parent->parentNode;
                        parent->isLeaf = true;
                        parent->~QuadtreeNode();
                    }
                }
            }
            leafs.erase(it);
        }
    }

    void cleanAndRetrieveData() {

    }
};
