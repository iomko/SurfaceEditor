#include "../src/Ray2D.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("Ray2D intersects a vertical segment", "[ray2d][basic]") {
    Ray2D ray;
    ray.origin = glm::vec2(0,0);
    ray.direction = glm::vec2(1,0);

    Segment2D seg;
    seg.p0 = glm::vec2(5,-1);
    seg.p1 = glm::vec2(5,1);

    glm::vec2 ip;
    REQUIRE(ray.intersectSegment(seg, ip) == true);
    REQUIRE(ip.x == Catch::Approx(5.0f));
    REQUIRE(ip.y == Catch::Approx(0.0f));
}

TEST_CASE("Ray2D misses when intersection outside segment", "[ray2d][edge]") {
    Ray2D ray;
    ray.origin = glm::vec2(0,0);
    ray.direction = glm::vec2(1,0);

    Segment2D seg;
    seg.p0 = glm::vec2(-5,1);
    seg.p1 = glm::vec2(-4,1);

    glm::vec2 ip;
    REQUIRE(ray.intersectSegment(seg, ip) == false);
}

TEST_CASE("Ray2D intersects at exactly a segment endpoint", "[ray2d][endpoint]") {
    Ray2D ray;
    ray.origin = glm::vec2(0,0);
    ray.direction = glm::vec2(1,0);

    Segment2D seg;
    seg.p0 = glm::vec2(5,0);
    seg.p1 = glm::vec2(10,0);

    glm::vec2 ip;
    REQUIRE(ray.intersectSegment(seg, ip) == true);
    REQUIRE(ip == glm::vec2(5.0f, 0.0f));
}

TEST_CASE("Ray2D intersects collinear segment when starting inside", "[ray2d][collinear]") {
    Ray2D ray;
    ray.origin = glm::vec2(5,0);
    ray.direction = glm::vec2(-1,0);

    Segment2D seg;
    seg.p0 = glm::vec2(0,0);
    seg.p1 = glm::vec2(10,0);

    glm::vec2 ip;
    REQUIRE(ray.intersectSegment(seg, ip) == true);
    REQUIRE(ip == glm::vec2(5.0f, 0.0f));
}

TEST_CASE("Ray2D parallel segment does not intersect", "[ray2d][parallel]") {
    Ray2D ray;
    ray.origin = glm::vec2(0,0);
    ray.direction = glm::vec2(1,0);

    Segment2D seg;
    seg.p0 = glm::vec2(0,1);
    seg.p1 = glm::vec2(10,1);

    glm::vec2 ip;
    REQUIRE(ray.intersectSegment(seg, ip) == false);
}
