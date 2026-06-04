#include "../src/Utils/GeometryUtils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("createPerpendicular2DVector returns a perpendicular direction", "[geometryutils]") {
    glm::vec2 result = utils::geometry::createPerpendicular2DVector(glm::vec2(0.0f), glm::vec2(1.0f, 0.0f));
    REQUIRE(result == glm::vec2(0.0f, 1.0f));
}

TEST_CASE("polygon2DOverlap detects overlap and non-overlap", "[geometryutils]") {
    std::vector<glm::vec2> a = {{0,0}, {2,0}, {2,2}, {0,2}};
    std::vector<glm::vec2> b = {{1,1}, {3,1}, {3,3}, {1,3}};
    std::vector<glm::vec2> c = {{2.1f,2.1f}, {3,2.1f}, {3,3}, {2.1f,3}};

    REQUIRE(utils::geometry::polygon2DOverlap(a, b));
    REQUIRE_FALSE(utils::geometry::polygon2DOverlap(a, c));
}

TEST_CASE("checkSideRelation returns same, opposite, and on-plane results", "[geometryutils]") {
    glm::vec3 normal(0.0f, 1.0f, 0.0f);
    glm::vec3 p1(0.0f, 1.0f, 0.0f);
    glm::vec3 p2(0.0f, 2.0f, 0.0f);
    glm::vec3 p3(0.0f, -1.0f, 0.0f);
    glm::vec3 p4(0.0f, 0.0f, 0.0f);

    REQUIRE(utils::geometry::checkSideRelation(normal, p1, p2) == utils::geometry::SameSide);
    REQUIRE(utils::geometry::checkSideRelation(normal, p1, p3) == utils::geometry::OppositeSide);
    REQUIRE(utils::geometry::checkSideRelation(normal, p1, p4) == utils::geometry::OnPlane);
}

TEST_CASE("getWindingOrder recognizes CW and CCW vertex orders", "[geometryutils]") {
    std::vector<glm::vec2> ccw = {{0,0}, {1,0}, {1,1}, {0,1}};
    std::vector<glm::vec2> cw = {{0,0}, {0,1}, {1,1}, {1,0}};
    std::vector<glm::vec2> invalid = {{0,0}, {1,0}};

    REQUIRE(utils::geometry::getWindingOrder(ccw) == utils::geometry::CCW);
    REQUIRE(utils::geometry::getWindingOrder(cw) == utils::geometry::CW);
    REQUIRE(utils::geometry::getWindingOrder(invalid) == utils::geometry::None);
}

TEST_CASE("getInwardNormal returns the correct inward vector", "[geometryutils]") {
    glm::vec2 first(0.0f, 0.0f);
    glm::vec2 second(1.0f, 0.0f);

    glm::vec2 inwardCCW = utils::geometry::getInwardNormal(first, second, utils::geometry::CCW);
    glm::vec2 inwardCW = utils::geometry::getInwardNormal(first, second, utils::geometry::CW);

    REQUIRE(inwardCCW == glm::vec2(-0.0f, 1.0f));
    REQUIRE(inwardCW == glm::vec2(0.0f, -1.0f));
}

TEST_CASE("projectVertices projects 3D points onto all supported axes", "[geometryutils]") {
    std::vector<glm::vec3> vertices = {{1,2,3}, {4,5,6}};
    auto zy = utils::geometry::projectVertices(vertices, utils::geometry::ProjectionAxis::ZY);
    auto xz = utils::geometry::projectVertices(vertices, utils::geometry::ProjectionAxis::XZ);
    auto xy = utils::geometry::projectVertices(vertices, utils::geometry::ProjectionAxis::XY);

    REQUIRE(zy[0] == glm::vec2(3,2));
    REQUIRE(xz[0] == glm::vec2(1,3));
    REQUIRE(xy[0] == glm::vec2(1,2));
}

TEST_CASE("computePolygonArea returns correct signed polygon area", "[geometryutils]") {
    std::vector<glm::vec2> triangle = {{0,0}, {1,0}, {0,1}};
    std::vector<glm::vec2> square = {{0,0}, {1,0}, {1,1}, {0,1}};

    REQUIRE(utils::geometry::computePolygonArea(triangle) == Catch::Approx(0.5f));
    REQUIRE(utils::geometry::computePolygonArea(square) == Catch::Approx(1.0f));
}

TEST_CASE("fitPlaneToPoints identifies coplanar points and returns normal", "[geometryutils]") {
    bool allCoplanar = false;
    std::vector<glm::vec3> triangle = {{0,0,0}, {1,0,0}, {0,1,0}};
    Plane plane = utils::geometry::fitPlaneToPoints(triangle, allCoplanar);

    REQUIRE(allCoplanar == true);
    REQUIRE(glm::dot(plane.normal, glm::vec3(0,0,1)) == Catch::Approx(1.0f));

    std::vector<glm::vec3> nonCoplanar = {{0,0,0}, {1,0,0}, {0,1,0}, {0,0,1}};
    Plane plane2 = utils::geometry::fitPlaneToPoints(nonCoplanar, allCoplanar);

    REQUIRE(allCoplanar == false);
    REQUIRE(glm::length(plane2.normal) > 0.0f);
}

TEST_CASE("intersectRayPlane returns hit only on valid intersection", "[geometryutils]") {
    Plane plane;
    plane.point = glm::vec3(0.0f, 0.0f, 0.0f);
    plane.normal = glm::vec3(0.0f, 1.0f, 0.0f);

    bool hit = false;
    glm::vec3 origin(0.0f, 1.0f, 0.0f);
    glm::vec3 direction(0.0f, -1.0f, 0.0f);

    glm::vec3 point = utils::geometry::intersectRayPlane(plane, origin, direction, hit);
    REQUIRE(hit);
    REQUIRE(point == glm::vec3(0.0f, 0.0f, 0.0f));

    glm::vec3 parallelDir(1.0f, 0.0f, 0.0f);
    glm::vec3 missed = utils::geometry::intersectRayPlane(plane, origin, parallelDir, hit);
    REQUIRE_FALSE(hit);
    REQUIRE(missed == glm::vec3(0.0f));
}
