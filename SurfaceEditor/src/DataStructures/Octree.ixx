//Octree.ixx:


module;
#include "../AABBBoundingRegion.h";
#include "HalfEdge.h";
#include <functional>;
export module DataStructures.Octree;
import <map>;
import <vector>;

export template<typename T>
class OctreeNode
{
public:
    AABBBoundingRegion nodeBounds;
    size_t depth = 0;
    size_t maxDepth = 1;
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
                    glm::vec3 centerBoundsPosition = nodeBounds.getMin() + ((nodeBounds.getMax() - nodeBounds.getMin()) * (1.0f / 2.0f));

                    childrenNodes[0] = new OctreeNode<T>(nodeBounds.getMin(), centerBoundsPosition, this->depth + 1, 0, this);
                    childrenNodes[1] = new OctreeNode<T>(glm::vec3(nodeBounds.getMin().x, nodeBounds.getMin().y, nodeBounds.getMax().z),
                        centerBoundsPosition, this->depth + 1, 1, this);
                    childrenNodes[2] = new OctreeNode<T>(glm::vec3(nodeBounds.getMax().x, nodeBounds.getMin().y, nodeBounds.getMin().z),
                        centerBoundsPosition, this->depth + 1, 2, this);
                    childrenNodes[3] = new OctreeNode<T>(glm::vec3(nodeBounds.getMax().x, nodeBounds.getMin().y, nodeBounds.getMax().z),
                        centerBoundsPosition, this->depth + 1, 3, this);
                    childrenNodes[4] = new OctreeNode<T>(glm::vec3(nodeBounds.getMin().x, nodeBounds.getMax().y, nodeBounds.getMin().z),
                        centerBoundsPosition, this->depth + 1, 4, this);
                    childrenNodes[5] = new OctreeNode<T>(glm::vec3(nodeBounds.getMin().x, nodeBounds.getMax().y, nodeBounds.getMax().z),
                        centerBoundsPosition, this->depth + 1, 5, this);
                    childrenNodes[6] = new OctreeNode<T>(glm::vec3(nodeBounds.getMax().x, nodeBounds.getMax().y, nodeBounds.getMin().z),
                        centerBoundsPosition, this->depth + 1, 6, this);
                    childrenNodes[7] = new OctreeNode<T>(nodeBounds.getMax(), centerBoundsPosition, this->depth + 1, 7, this);

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

    void findData(const Ray& ray, std::vector<OctreeNode<T>*>& acumulatedOctreeNodes) {
        if (nodeBounds.intersectsRay(ray)) {
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


export template <typename Octree>
class OctreeIterator {
public:
    using ValueType = typename Octree::ValueType;
    using PointerType = typename ValueType*;
    using ReferenceType = typename ValueType&;

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


export template<class T>
class Octree
{
public:
    std::map<T, std::vector<OctreeNode<T>*>> leafs;
    using ValueType = OctreeNode<T>;
    using OctreeIterator = OctreeIterator<Octree<T>>;
    OctreeNode<T>* rootNode;

    Octree(glm::vec3 minVector, glm::vec3 maxVector) : rootNode(new OctreeNode<T>(minVector, maxVector, 0, 0))
    {}
    /*
    Octree(Octree<T>&& other) noexcept
        : rootNode(std::move(other.rootNode)) {}
    */

    /*
    bool operator<(const Octree<T>& other) const {
        return rootNode->nodeBounds.getMin() < other.rootNode->nodeBounds.getMin();
    }
    */

    Octree(Octree<T>&& other) noexcept
        : rootNode(nullptr), leafs(std::move(other.leafs)) {
        rootNode = other.rootNode;
        other.rootNode = nullptr;
    }

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
        rootNode->addData(data, dataBounds, this->leafs);
    }
    //unhide later

    //potreboval by som aby mi pouzivatel zadal aky algoritmus mam urobit ked mu dam k dispozicii data v octreeNode a ray

    std::tuple<OctreeNode<T>*, T, float> findDataInOctree(const Ray& ray, const std::function<std::pair<bool, float>(const T&, const Ray&)>& algorithm) {
        T returnData;
        OctreeNode<T>* closestHitNode = nullptr;
        float minHitDistance = std::numeric_limits<float>::max();
        std::vector<OctreeNode<T>*> hitOctreeLiefs;
        
        if (rootNode->nodeBounds.intersectsRay(ray)) {
            rootNode->findData(ray, hitOctreeLiefs);
        }
        for (const auto& hitOctreeLief : hitOctreeLiefs)
        {
	        for (const auto& data : hitOctreeLief->nodeData)
	        {
                const auto& distanceHitPair = algorithm(data, ray);
                if(distanceHitPair.first)
                {
                    if(distanceHitPair.second < minHitDistance)
                    {
                        minHitDistance = distanceHitPair.second;
                        returnData = data;
                        closestHitNode = hitOctreeLief;
                    }

                }
	        }
        }
        return std::make_tuple(closestHitNode, returnData, minHitDistance);
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




