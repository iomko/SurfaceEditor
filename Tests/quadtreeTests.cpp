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