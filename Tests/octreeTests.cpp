#include "../src/Structures/Octree.h"

#include <catch2/catch_test_macros.hpp>

#include <set>
#include <random>

AABBBoundingRegion makeBoxOctree(float min, float max) {
    return AABBBoundingRegion(glm::vec3(min), glm::vec3(max));
}

auto boundsFuncOctree = std::function<bool(const AABBBoundingRegion&, const AABBBoundingRegion&)>(
    [](const AABBBoundingRegion& a, const AABBBoundingRegion& b) {
        return a.intersectsAABB(b);
    }
);

TEST_CASE("Octree initializes correctly", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    REQUIRE(tree.rootNode != nullptr);
    REQUIRE(tree.isEmpty());
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}
TEST_CASE("Insert single element (multi-leaf)", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(1, makeBoxOctree(1,2));

    REQUIRE_FALSE(tree.isEmpty());
    REQUIRE(tree.rootNode->dataCount >= 1); // not exactly 1 anymore
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}
TEST_CASE("Insert multiple elements (multi-leaf)", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    for (int i = 0; i < 10; ++i) {
        tree.addDataToOctree(i, makeBoxOctree(i, i+0.5f));
    }

    REQUIRE(tree.rootNode->dataCount >= 10);
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}
TEST_CASE("Find data returns correct unique results", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(1, makeBoxOctree(1,2));
    tree.addDataToOctree(2, makeBoxOctree(5,6));

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(0,3),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
            [&](const int& data, const AABBBoundingRegion&) {
                result.insert(data);
                return true;
            }
        )
    );

    REQUIRE(result.size() == 1);
    REQUIRE(result.count(1) == 1);
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}
TEST_CASE("Query outside region returns nothing", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(1, makeBoxOctree(1,2));

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(20,30),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
            [&](const int& data, const AABBBoundingRegion&) {
                result.insert(data);
                return true;
            }
        )
    );

    REQUIRE(result.empty());
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}
TEST_CASE("Overlapping object deduplicates correctly", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(42, makeBoxOctree(0,10)); // overlaps everything

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(0,10),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
            [&](const int& data, const AABBBoundingRegion&) {
                result.insert(data);
                return true;
            }
        )
    );

    REQUIRE(result.size() == 1);
    REQUIRE(result.count(42) == 1);
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}
TEST_CASE("Remove element removes all references", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(1, makeBoxOctree(1,2));
    tree.removeData(1);

    REQUIRE(tree.isEmpty());
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}
TEST_CASE("Remove non-existing element does nothing", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.removeData(999); // should not crash

    REQUIRE(tree.isEmpty());
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}
TEST_CASE("Zero-size AABB handled", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(1, makeBoxOctree(5,5));

    REQUIRE(tree.rootNode->dataCount >= 1);
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}
TEST_CASE("Mass insert stress (unique correctness)", "[octree][stress]") {
    Octree<int> tree({0,0,0}, {100,100,100});

    for (int i = 0; i < 1000; ++i) {
        tree.addDataToOctree(i, makeBoxOctree(i % 50, (i % 50) + 1));
    }

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(0,100),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
            [&](const int& data, const AABBBoundingRegion&) {
                result.insert(data);
                return true;
            }
        )
    );

    REQUIRE(result.size() == 1000);
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}
TEST_CASE("Iterator traverses without crashing", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(1, makeBoxOctree(1,2));

    int visited = 0;

    for (auto it = tree.begin(); it != tree.end(); ++it) {
        visited++;
    }

    REQUIRE(visited > 0);
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}

TEST_CASE("Object exactly on boundary is handled correctly", "[octree][edge]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    // exactly on split boundary
    tree.addDataToOctree(1, makeBoxOctree(5,5));

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(5,5),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
            [&](const int& data, const AABBBoundingRegion&) {
                result.insert(data);
                return true;
            }
        )
    );

    REQUIRE(result.count(1) == 1);
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}

TEST_CASE("Object spanning multiple octants is found once", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    // crosses center (5,5,5)
    tree.addDataToOctree(99, makeBoxOctree(4,6));

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(0,10),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
        [&](const int& data, const AABBBoundingRegion&) {
            result.insert(data);
            return true;
        })
    );

    REQUIRE(result.size() == 1);
    REQUIRE(result.count(99) == 1);
}

TEST_CASE("All objects at same position", "[octree][edge]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    for (int i = 0; i < 50; ++i) {
        tree.addDataToOctree(i, makeBoxOctree(5,5));
    }

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(5,5),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
        [&](const int& data, const AABBBoundingRegion&) {
            result.insert(data);
            return true;
        })
    );

    REQUIRE(result.size() == 50);
}

TEST_CASE("Thin AABB is handled correctly", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    // almost zero thickness
    tree.addDataToOctree(1, AABBBoundingRegion(
        {5,5,5},
        {5.000001f,5.000001f,5.000001f}
    ));

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(5,6),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
        [&](const int& data, const AABBBoundingRegion&) {
            result.insert(data);
            return true;
        })
    );

    REQUIRE(result.count(1) == 1);
}

TEST_CASE("Insert outside root bounds", "[octree][edge]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    // completely outside
    tree.addDataToOctree(1, makeBoxOctree(20,30));

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(0,10),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
        [&](const int& data, const AABBBoundingRegion&) {
            result.insert(data);
            return true;
        })
    );

    // depends on your design:
    REQUIRE(result.empty());
}

TEST_CASE("Remove object spanning multiple nodes", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(1, makeBoxOctree(0,10)); // everywhere

    tree.removeData(1);

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(0,10),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
        [&](const int& data, const AABBBoundingRegion&) {
            result.insert(data);
            return true;
        })
    );

    REQUIRE(result.empty());
}

TEST_CASE("Reinsert after removal works correctly", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(1, makeBoxOctree(1,2));
    tree.removeData(1);
    tree.addDataToOctree(1, makeBoxOctree(3,4));

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(0,10),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
        [&](const int& data, const AABBBoundingRegion&) {
            result.insert(data);
            return true;
        })
    );

    REQUIRE(result.count(1) == 1);
}
TEST_CASE("Query exactly matches root bounds", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(1, makeBoxOctree(2,3));

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(0,10),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
        [&](const int& data, const AABBBoundingRegion&) {
            result.insert(data);
            return true;
        })
    );

    REQUIRE(result.count(1) == 1);
}

TEST_CASE("Random insert/remove consistency test", "[octree][random]") {
    Octree<int> tree({0,0,0}, {100,100,100});

    std::mt19937 rng(1337);
    std::uniform_real_distribution<float> distPos(0.0f, 100.0f);
    std::uniform_int_distribution<int> distVal(1, 10000);

    const int INSERT_COUNT = 20000;
    const int REMOVE_COUNT = 10000;

    // Track what SHOULD exist
    std::set<int> inserted;
    std::vector<int> insertionOrder;

    // 🔹 Step 1: Insert random elements
    for (int i = 0; i < INSERT_COUNT; ++i) {
        int value = distVal(rng);

        float min = distPos(rng);
        float max = min + 1.0f; // small AABB

        tree.addDataToOctree(value, makeBoxOctree(min, max));

        inserted.insert(value);
        insertionOrder.push_back(value);
    }

    REQUIRE_FALSE(inserted.empty());

    // 🔹 Step 2: Randomly remove some of them
    std::shuffle(insertionOrder.begin(), insertionOrder.end(), rng);

    for (int i = 0; i < REMOVE_COUNT && i < insertionOrder.size(); ++i) {
        int valToRemove = insertionOrder[i];

        tree.removeData(valToRemove);
        inserted.erase(valToRemove);
    }

    // 🔹 Step 3: Pick one element that SHOULD still exist
    REQUIRE_FALSE(inserted.empty());

    int target = *inserted.begin();

    // 🔹 Step 4: Query whole space and collect results
    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(0, 100),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
            [&](const int& data, const AABBBoundingRegion&) {
                result.insert(data);
                return true;
            }
        )
    );

    // 🔹 Step 5: Validate
    REQUIRE(result.count(target) == 1);
    REQUIRE(result.size() == inserted.size());
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}

TEST_CASE("Invalid inverted AABB", "[octree][edge]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    REQUIRE_NOTHROW(tree.addDataToOctree(1, AABBBoundingRegion({10,10,10}, {0,0,0})));
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}

TEST_CASE("Max depth enforcement", "[octree][edge]") {
    Octree<int> tree({0,0,0}, {100,100,100});

    for (int i = 0; i < 1000; ++i) {
        tree.addDataToOctree(i, makeBoxOctree(i, i+0.1f));
    }

    REQUIRE(tree.rootNode->depth == 0);
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}

TEST_CASE("Split invariant correctness", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    for (int i = 0; i < 100; ++i)
        tree.addDataToOctree(i, makeBoxOctree(i % 10, (i % 10) + 0.1f));

    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}

TEST_CASE("Remove triggers node cleanup", "[octree][edge]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(1, makeBoxOctree(1,2));
    tree.addDataToOctree(2, makeBoxOctree(1,2));

    tree.removeData(1);
    tree.removeData(2);

    REQUIRE(tree.rootNode->dataCount == 0);
    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}

TEST_CASE("Remove under heavy subdivision", "[octree][stress]") {
    Octree<int> tree({0,0,0}, {100,100,100});

    for (int i = 0; i < 5000; ++i)
        tree.addDataToOctree(i, makeBoxOctree(i % 20, i % 20 + 0.5f));

    for (int i = 0; i < 2500; ++i)
        tree.removeData(i);

    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}

TEST_CASE("Query empty octree", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(0,10),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
            [&](const int& d, const AABBBoundingRegion&) {
                result.insert(d);
                return true;
            }
        )
    );

    REQUIRE(result.empty());
}

TEST_CASE("Partial overlap query correctness", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(1, makeBoxOctree(4,6));
    tree.addDataToOctree(2, makeBoxOctree(6,8));

    std::set<int> result;

    tree.findDataInOctree(
        makeBoxOctree(5,5),
        boundsFuncOctree,
        std::function<bool(const int&, const AABBBoundingRegion&)>(
            [&](const int& d, const AABBBoundingRegion&) {
                result.insert(d);
                return true;
            }
        )
    );

    REQUIRE(result.size() >= 1);
}

TEST_CASE("Iterator on empty tree", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    int count = 0;
    for (auto it = tree.begin(); it != tree.end(); ++it)
        count++;

    REQUIRE(count == 0);
}

TEST_CASE("Iterator single element", "[octree]") {
    Octree<int> tree({0,0,0}, {10,10,10});

    tree.addDataToOctree(1, makeBoxOctree(1,2));

    int count = 0;
    for (auto it = tree.begin(); it != tree.end(); ++it)
        count++;

    REQUIRE(count >= 1);
}

TEST_CASE("Random fuzz stability", "[octree][stress]") {
    Octree<int> tree({0,0,0}, {100,100,100});

    std::mt19937 rng(42);
    std::uniform_real_distribution<float> d(0, 100);
    std::uniform_int_distribution<int> v(0, 1000);

    std::vector<int> inserted;

    for (int i = 0; i < 10000; ++i) {
        int val = v(rng);
        float a = d(rng);

        tree.addDataToOctree(val, makeBoxOctree(a, a + 0.1f));
        inserted.push_back(val);

        if (i % 3 == 0 && !inserted.empty()) {
            tree.removeData(inserted.back());
            inserted.pop_back();
        }
    }

    REQUIRE(tree.validateOctreeNode(tree.rootNode));
}