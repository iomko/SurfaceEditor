#include "../src/Primitives/AABB.h"
#include "../src/Primitives/Sphere.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("AABB basic contains and getters", "[aabb][basic]") {
    AABBBoundingRegion a(glm::vec3(0,0,0), glm::vec3(1,1,1));

    REQUIRE(a.getMinBoundsPos() == glm::vec3(0,0,0));
    REQUIRE(a.getMaxBoundsPos() == glm::vec3(1,1,1));
    REQUIRE(a.containsPoint(glm::vec3(0.5f,0.5f,0.5f)) == true);
    REQUIRE(a.containsPoint(glm::vec3(2,2,2)) == false);
}

TEST_CASE("AABB intersects sphere", "[aabb][sphere]") {
    AABBBoundingRegion a(glm::vec3(0,0,0), glm::vec3(2,2,2));
    Sphere s;
    s.position = glm::vec3(1,1,1);
    s.radius = 0.5f;

    REQUIRE(a.intersectsSphere(s) == true);

    s.position = glm::vec3(10,10,10);
    REQUIRE(a.intersectsSphere(s) == false);
}

TEST_CASE("AABB intersects other AABB and comparison", "[aabb][compare]") {
    AABBBoundingRegion a(glm::vec3(0,0,0), glm::vec3(2,2,2));
    AABBBoundingRegion b(glm::vec3(1,1,1), glm::vec3(3,3,3));
    AABBBoundingRegion c(glm::vec3(3.1f,3.1f,3.1f), glm::vec3(4,4,4));

    REQUIRE(a.intersectsAABB(b) == true);
    REQUIRE(a.intersectsAABB(c) == false);

    // copy and assign
    AABBBoundingRegion copy = a;
    REQUIRE(copy == a);
    copy = b;
    REQUIRE(copy == b);

    if(a < b) {
        REQUIRE_FALSE(b < a);
    } else if (b < a) {
        REQUIRE_FALSE(a < b);
    } else {
        FAIL("A and B should not be equal");
    }
    //REQUIRE((a < b) == true || (b < a) == true);
}

TEST_CASE("AABB intersects ray", "[aabb][ray]") {
    AABBBoundingRegion a(glm::vec3(0,0,0), glm::vec3(2,2,2));

    Ray ray(glm::vec3(-1,1,1), glm::vec3(1,0,0));
    REQUIRE(a.intersectsRay(ray) == true);

    Ray miss(glm::vec3(-1,3,3), glm::vec3(1,0,0));
    REQUIRE(a.intersectsRay(miss) == false);
}

TEST_CASE("AABB plane intersection: plane cutting box", "[aabb][plane]") {
    Plane p;
    p.point = glm::vec3(1,0,0);
    p.normal = glm::vec3(1,0,0);

    AABBBoundingRegion a(glm::vec3(0,0,0), glm::vec3(2,2,2));
    REQUIRE(a.insersectsPlane(p) == true);

    Plane p2;
    p2.point = glm::vec3(10,0,0);
    p2.normal = glm::vec3(1,0,0);
    REQUIRE(a.insersectsPlane(p2) == false);
}

TEST_CASE("AABB setBounds normalizes swapped bounds and contains nested AABBs", "[aabb][setBounds]") {
    AABBBoundingRegion a;
    a.setBounds(glm::vec3(2,2,2), glm::vec3(-1,-1,-1));

    REQUIRE(a.getMinBoundsPos() == glm::vec3(-1,-1,-1));
    REQUIRE(a.getMaxBoundsPos() == glm::vec3(2,2,2));

    AABBBoundingRegion inner(glm::vec3(0,0,0), glm::vec3(1,1,1));
    AABBBoundingRegion outer(glm::vec3(-2,-2,-2), glm::vec3(3,3,3));

    REQUIRE(outer.containsAABB(a) == true);
    REQUIRE(outer.containsAABB(inner) == true);
    REQUIRE(a.containsAABB(outer) == false);
    REQUIRE(a != outer);
}

TEST_CASE("AABB template constructor computes bounds from an extractor", "[aabb][template]") {
    std::vector<glm::vec3> points = {
        {1.0f, 2.0f, 3.0f},
        {-1.0f, 4.0f, 0.0f},
        {2.0f, -2.0f, 5.0f}
    };

    AABBBoundingRegion a(points.begin(), points.end(), [](const glm::vec3& p) {
        return p;
    });

    REQUIRE(a.getMinBoundsPos() == glm::vec3(-1.0f, -2.0f, 0.0f));
    REQUIRE(a.getMaxBoundsPos() == glm::vec3(2.0f, 4.0f, 5.0f));
}

TEST_CASE("AABB ray intersection returns true when ray starts inside", "[aabb][ray][inside]") {
    AABBBoundingRegion a(glm::vec3(0.0f), glm::vec3(1.0f));
    Ray ray(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f));

    REQUIRE(a.intersectsRay(ray) == true);
}

TEST_CASE("AABB plane intersection handles negative normals", "[aabb][plane][negative]") {
    Plane p;
    p.point = glm::vec3(1.0f, 0.0f, 0.0f);
    p.normal = glm::vec3(-1.0f, 0.0f, 0.0f);

    AABBBoundingRegion a(glm::vec3(0.0f), glm::vec3(2.0f));
    REQUIRE(a.insersectsPlane(p) == true);
}
