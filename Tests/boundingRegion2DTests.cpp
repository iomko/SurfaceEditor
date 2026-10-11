#include "../src/Primitives/BoundingRegion2D.h"
#include "../src/Ray2D.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("BoundingRegion2D template constructor computes min and max correctly", "[boundingregion2d][constructor]") {
    std::vector<glm::vec2> points = {{1.0f, 2.0f}, {3.0f, -1.0f}, {-2.0f, 5.0f}};
    BoundingRegion2D region(points.begin(), points.end(), [](const glm::vec2& p) {
        return p;
    });

    REQUIRE(region.getMinBoundsPos() == glm::vec2(-2.0f, -1.0f));
    REQUIRE(region.getMaxBoundsPos() == glm::vec2(3.0f, 5.0f));
}

TEST_CASE("BoundingRegion2D copy and assignment preserve bounds", "[boundingregion2d][copy]") {
    BoundingRegion2D original(glm::vec2(0.0f, 1.0f), glm::vec2(2.0f, 4.0f));
    BoundingRegion2D copy(original);
    BoundingRegion2D assigned(glm::vec2(1.0f), glm::vec2(1.0f));
    assigned = original;

    REQUIRE(copy == original);
    REQUIRE(assigned == original);
}

TEST_CASE("BoundingRegion2D intersection queries work as expected", "[boundingregion2d][intersection]") {
    BoundingRegion2D a(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 2.0f));
    BoundingRegion2D b(glm::vec2(1.0f, 1.0f), glm::vec2(3.0f, 3.0f));
    BoundingRegion2D c(glm::vec2(2.1f, 2.1f), glm::vec2(4.0f, 4.0f));

    REQUIRE(a.intersectsBoundingRegion2D(b));
    REQUIRE_FALSE(a.intersectsBoundingRegion2D(c));

    Ray2D hitRay;
    hitRay.origin = glm::vec2(-1.0f, 1.0f);
    hitRay.direction = glm::vec2(1.0f, 0.5f);

    Ray2D missRay;
    missRay.origin = glm::vec2(-1.0f, 3.0f);
    missRay.direction = glm::vec2(1.0f, 0.5f);

    REQUIRE(a.intersectsRay(hitRay));
    REQUIRE_FALSE(a.intersectsRay(missRay));

    Ray2D insideRay;
    insideRay.origin = glm::vec2(1.0f, 1.0f);
    insideRay.direction = glm::vec2(0.1f, 0.1f);
    REQUIRE(a.intersectsRay(insideRay));
}
