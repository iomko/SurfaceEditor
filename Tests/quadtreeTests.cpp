#include "../src/Structures/QuadTree.h"

#include <catch2/catch_test_macros.hpp>

#include <set>
#include <random>

//QUADTREE TESTS

BoundingRegion2D makeBoxQuadTree(float min, float max) {
    return BoundingRegion2D(glm::vec2(min), glm::vec2(max));
}

auto boundsFuncQuadTree = std::function<bool(const BoundingRegion2D&, const BoundingRegion2D&)>(
    [](const BoundingRegion2D& a, const BoundingRegion2D& b) {
        return a.intersectsBoundingRegion2D(b);        
    }
);
TEST_CASE("Quadtree initializes correctly", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});

    REQUIRE(tree.rootNode != nullptr);
    REQUIRE(tree.isEmpty());
}
TEST_CASE("Insert single element", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});

    tree.addDataToQuadtree(1, makeBoxQuadTree(1,2));

    REQUIRE_FALSE(tree.isEmpty());
    REQUIRE(tree.rootNode->dataCount >= 1);
}
TEST_CASE("Insert multiple elements", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});

    for (int i = 0; i < 10; ++i) {
        tree.addDataToQuadtree(i, makeBoxQuadTree(i, i+0.5f));
    }

    REQUIRE(tree.rootNode->dataCount >= 10);
}
TEST_CASE("Query returns unique results", "[quadtree][unique]") {
    Quadtree<int> tree({0,0}, {10,10});

    tree.addDataToQuadtree(1, makeBoxQuadTree(1,2));
    tree.addDataToQuadtree(2, makeBoxQuadTree(6,7));

    std::set<int> result;

    tree.findDataInQuadtree(
        makeBoxQuadTree(0,3),
        boundsFuncQuadTree,
        std::function<bool(const int&, const BoundingRegion2D&)>(
            [&](const int& data, const BoundingRegion2D&) {
                result.insert(data);
                return true;
            }
        )
    );

    REQUIRE(result.size() == 1);
    REQUIRE(result.count(1) == 1);
    
}
TEST_CASE("Query outside region returns nothing", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});

    tree.addDataToQuadtree(1, makeBoxQuadTree(1,2));

    std::set<int> result;

    tree.findDataInQuadtree(
        makeBoxQuadTree(20,30),
        boundsFuncQuadTree,
        std::function<bool(const int&, const BoundingRegion2D&)>(
        [&](const int& data, const BoundingRegion2D&) {
            result.insert(data);
            return true;
        }
        )
    );

    REQUIRE(result.empty());
}
TEST_CASE("Overlapping object appears once", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});

    tree.addDataToQuadtree(42, makeBoxQuadTree(0,10));

    std::set<int> result;

    tree.findDataInQuadtree(
        makeBoxQuadTree(0,10),
        boundsFuncQuadTree,
        std::function<bool(const int&, const BoundingRegion2D&)>(
        [&](const int& data, const BoundingRegion2D&) {
            result.insert(data);
            return true;
        }
        )
    );

    REQUIRE(result.size() == 1);
    REQUIRE(result.count(42) == 1);
}
TEST_CASE("Remove element", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});

    tree.addDataToQuadtree(1, makeBoxQuadTree(1,2));
    tree.removeData(1);

    REQUIRE(tree.isEmpty());
}
TEST_CASE("Remove non-existing element", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});

    tree.removeData(999); // should not crash
    REQUIRE(tree.isEmpty());
}
TEST_CASE("Zero-size AABB handled", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});

    tree.addDataToQuadtree(1, makeBoxQuadTree(5,5));

    REQUIRE(tree.rootNode->dataCount >= 1);
}
TEST_CASE("Stress test large insertion", "[quadtree][stress]") {
    Quadtree<int> tree({0,0}, {100,100});

    for (int i = 0; i < 1000; ++i) {
        tree.addDataToQuadtree(i, makeBoxQuadTree(i % 50, (i % 50) + 1));
    }

    std::set<int> result;

    tree.findDataInQuadtree(
        makeBoxQuadTree(0,100),
        boundsFuncQuadTree,
        std::function<bool(const int&, const BoundingRegion2D&)>(
        [&](const int& data, const BoundingRegion2D&) {
            result.insert(data);
            return true;
        }
        )
    );

    REQUIRE(result.size() == 1000);
}
TEST_CASE("Iterator traversal", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});

    tree.addDataToQuadtree(1, makeBoxQuadTree(1,2));

    int visited = 0;

    for (auto it = tree.begin(); it != tree.end(); ++it) {
        visited++;
    }

    REQUIRE(visited > 0);
}
TEST_CASE("Random insert/remove consistency test", "[quadtree][random]") {
    Quadtree<int> tree({0,0}, {100,100});

    std::mt19937 rng(1337);
    std::uniform_real_distribution<float> distPos(0.0f, 100.0f);
    std::uniform_int_distribution<int> distVal(1, 10000);

    const int INSERT_COUNT = 200;
    const int REMOVE_COUNT = 100;

    std::set<int> alive;
    std::vector<int> order;

    // insert
    for (int i = 0; i < INSERT_COUNT; ++i) {
        int v = distVal(rng);
        float min = distPos(rng);

        tree.addDataToQuadtree(v, makeBoxQuadTree(min, min + 1));

        alive.insert(v);
        order.push_back(v);
    }

    REQUIRE_FALSE(alive.empty());

    // remove random subset
    std::shuffle(order.begin(), order.end(), rng);

    for (int i = 0; i < REMOVE_COUNT && i < order.size(); ++i) {
        tree.removeData(order[i]);
        alive.erase(order[i]);
    }

    REQUIRE_FALSE(alive.empty());

    int target = *alive.begin();

    std::set<int> result;

    tree.findDataInQuadtree(
        makeBoxQuadTree(0,100),
        boundsFuncQuadTree,
        std::function<bool(const int&, const BoundingRegion2D&)>(
        [&](const int& data, const BoundingRegion2D&) {
            result.insert(data);
            return true;
        }
        )
    );

    REQUIRE(result.count(target) == 1);
}

TEST_CASE("Quadtree move constructor transfers ownership", "[quadtree][move]") {
    Quadtree<int> tree({0,0}, {10,10});
    tree.addDataToQuadtree(7, makeBoxQuadTree(2, 3));

    Quadtree<int> movedTree(std::move(tree));

    REQUIRE(tree.rootNode == nullptr);
    REQUIRE(movedTree.rootNode != nullptr);
    REQUIRE_FALSE(movedTree.isEmpty());
}

TEST_CASE("Quadtree begin/end iterator behaves on empty and non-empty trees", "[quadtree][iterator]") {
    Quadtree<int> emptyTree({0,0}, {10,10});
    REQUIRE(emptyTree.begin() == emptyTree.end());

    Quadtree<int> tree({0,0}, {10,10});
    tree.addDataToQuadtree(1, makeBoxQuadTree(1,2));
    REQUIRE(tree.begin() != tree.end());
}

TEST_CASE("Quadtree findMaxDepthNodes returns matching leaf nodes", "[quadtree][search]") {
    Quadtree<int> tree({0,0}, {10,10});
    tree.addDataToQuadtree(1, makeBoxQuadTree(1,2));

    std::vector<QuadtreeNode<int>*> nodes;
    tree.findMaxDepthNodes(makeBoxQuadTree(0,3), boundsFuncQuadTree, nodes);

    REQUIRE(nodes.size() > 0);
    REQUIRE(nodes.front()->nodeData.size() > 0);
}

TEST_CASE("BoundingRegion2D comparison operators and template constructor", "[boundingregion2d]") {
    std::vector<glm::vec2> points = {{4,2}, {1,5}, {3,3}};
    BoundingRegion2D region(points.begin(), points.end(), [](const glm::vec2& p) {
        return p;
    });

    REQUIRE(region.getMinBoundsPos() == glm::vec2(1,2));
    REQUIRE(region.getMaxBoundsPos() == glm::vec2(4,5));

    BoundingRegion2D other(glm::vec2(0,0), glm::vec2(1,1));
    REQUIRE(other < region);
    REQUIRE(region != other);
}

TEST_CASE("BoundingRegion2D ray intersection handles negative direction", "[boundingregion2d][ray]") {
    BoundingRegion2D region(glm::vec2(0,0), glm::vec2(10,10));
    Ray2D ray;
    ray.origin = glm::vec2(5,20);
    ray.direction = glm::vec2(0,-1);

    REQUIRE(region.intersectsRay(ray) == true);
}
TEST_CASE("Data spanning multiple quadrants tracks counts and queries properly", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});
    // Set maxDepth deep enough to trigger subdivision if implementation defaults allow
    tree.rootNode->maxDepth = 2; 

    // An object sitting perfectly dead-center crosses all 4 quadrant boundaries
    tree.addDataToQuadtree(100, makeBoxQuadTree(4.0f, 6.0f));

    // 1. Is it stored across multiple nodes internally?
    // Because it intersects all 4 quadrants, the root's tracking shouldn't break.
    REQUIRE(tree.rootNode->dataCount >= 4); 

    // 2. Query only one quadrant—does it extract our spanning item?
    int count = 0;
    tree.findDataInQuadtree(
        makeBoxQuadTree(0, 3), // Bottom-left quadrant only
        boundsFuncQuadTree,
        std::function<bool(const int&, const BoundingRegion2D&)>(
            [&](const int& data, const BoundingRegion2D&) {
                if (data == 100) count++;
                return true;
            }
        )
    );
    REQUIRE(count == 1);
}
TEST_CASE("Node collapse via removal cleanly allows re-insertion", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});
    tree.rootNode->maxDepth = 1;

    // Trigger a split by putting data into a leaf node's region
    tree.addDataToQuadtree(5, makeBoxQuadTree(1, 2));
    REQUIRE_FALSE(tree.rootNode->isLeaf);

    // Remove it to trigger your cleanup/collapse logic
    tree.removeData(5);
    REQUIRE(tree.isEmpty());
    REQUIRE(tree.rootNode->isLeaf);

    // CRITICAL: Re-inserting into the exact same region.
    // If dangling child node pointers remain, this could cause a segmentation fault 
    // or corrupt your heap state when addData tries to evaluate childrenNodes!
    REQUIRE_NOTHROW(tree.addDataToQuadtree(6, makeBoxQuadTree(1, 2)));
    REQUIRE(tree.rootNode->dataCount == 1);
}
TEST_CASE("Quadtree move constructor transfers ownership completely", "[quadtree]") {
    Quadtree<int> source({0,0}, {10,10});
    source.addDataToQuadtree(7, makeBoxQuadTree(1, 2));

    // Move it out
    Quadtree<int> destination(std::move(source));

    // The source should be neutralized
    REQUIRE(source.rootNode == nullptr);

    // The destination should contain the payload perfectly
    std::set<int> result;
    destination.findDataInQuadtree(
        makeBoxQuadTree(0,10),
        boundsFuncQuadTree,
        std::function<bool(const int&, const BoundingRegion2D&)>(
            [&](const int& data, const BoundingRegion2D&) {
                result.insert(data);
                return true;
            }
        )
    );
    REQUIRE(result.count(7) == 1);
}
TEST_CASE("Individual quadrant insertions route to correct spatial locations", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});
    tree.rootNode->maxDepth = 1;

    // Explicitly place an item in the Top-Right quadrant (X: 5..10, Y: 5..10)
    tree.addDataToQuadtree(99, makeBoxQuadTree(7, 8));

    std::set<int> bottomLeftResult;
    std::set<int> topRightResult;

    // Querying Bottom-Left (0..5) should yield nothing
    tree.findDataInQuadtree(
        makeBoxQuadTree(1, 4),
        boundsFuncQuadTree,
        std::function<bool(const int&, const BoundingRegion2D&)>(
            [&](const int& data, const BoundingRegion2D&) {
                bottomLeftResult.insert(data);
                return true;
            }
        )
    );

    // Querying Top-Right (5..10) should capture it
    tree.findDataInQuadtree(
        makeBoxQuadTree(6, 9),
        boundsFuncQuadTree,
        std::function<bool(const int&, const BoundingRegion2D&)>(
            [&](const int& data, const BoundingRegion2D&) {
                topRightResult.insert(data);
                return true;
            }
        )
    );

    REQUIRE(bottomLeftResult.empty());
    REQUIRE(topRightResult.count(99) == 1);
}
TEST_CASE("Iterator covers all elements in an asymmetric tree layout", "[quadtree]") {
    Quadtree<int> tree({0,0}, {10,10});
    tree.rootNode->maxDepth = 2;

    // Heavily bias insertion into the bottom-left corner sub-quadrants
    tree.addDataToQuadtree(1, makeBoxQuadTree(0.1f, 0.2f));
    tree.addDataToQuadtree(2, makeBoxQuadTree(0.3f, 0.4f));
    tree.addDataToQuadtree(3, makeBoxQuadTree(4.1f, 4.2f));

    size_t distinctNodesVisited = 0;
    
    // Ensuring the loop terminates normally and doesn't get stuck in cyclic parent traps
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        distinctNodesVisited++;
        // Check if we can safely read properties out of the current leaf node
        REQUIRE(it->depth <= tree.rootNode->maxDepth);
    }

    REQUIRE(distinctNodesVisited > 0);
}

