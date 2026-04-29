#include "../src/Scene/Mesh.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <set>
#include <random>

static std::vector<std::vector<int>> triangle = {
    {0, 1, 2}
};

//
// 🔹 BASIC VALID CASE
//
TEST_CASE("Mesh builds simple triangle", "[mesh][basic]") {
    std::vector<glm::vec3> vertices = {
        {0,0,0}, {1,0,0}, {0,1,0}
    };

    Mesh mesh(triangle, vertices);

    REQUIRE(mesh.getHalfEdgeStructure() != nullptr);

    auto* he = mesh.getHalfEdgeStructure();
    REQUIRE(he->m_vertices.size() == 3);
    REQUIRE(he->m_edges.size() == 3);
    REQUIRE(he->m_faces.size() == 1);
}

//
// 🔹 EMPTY INPUT
//
TEST_CASE("Mesh handles empty input", "[mesh][edgecase]") {
    std::vector<std::vector<int>> polygons;
    std::vector<glm::vec3> vertices;

    Mesh mesh(polygons, vertices);

    auto* he = mesh.getHalfEdgeStructure();

    REQUIRE(he->m_vertices.empty());
    REQUIRE(he->m_edges.empty());
    REQUIRE(he->m_faces.empty());
}

//
// 🔹 SINGLE VERTEX (INVALID POLYGON)
//
TEST_CASE("Mesh ignores invalid polygons (<3 vertices)", "[mesh][edgecase]") {
    std::vector<std::vector<int>> polygons = {
        {0}, {0,1}
    };

    std::vector<glm::vec3> vertices = {
        {0,0,0}, {1,0,0}
    };

    Mesh mesh(polygons, vertices);

    auto* he = mesh.getHalfEdgeStructure();

    REQUIRE(he->m_faces.empty());
}

//
// 🔹 DEGENERATE TRIANGLE (COLLINEAR)
//
TEST_CASE("Mesh handles collinear triangle", "[mesh][edgecase]") {
    std::vector<glm::vec3> vertices = {
        {0,0,0}, {1,0,0}, {2,0,0}
    };

    Mesh mesh(triangle, vertices);

    auto bounds = mesh.m_meshBounds;

    REQUIRE(bounds.getMinBoundsPos().x == Catch::Approx(0).margin(0.0001));
    REQUIRE(bounds.getMaxBoundsPos().x == Catch::Approx(2).margin(0.0001));
}

//
// 🔹 ALL POINTS SAME
//
TEST_CASE("Mesh handles identical vertices", "[mesh][edgecase]") {
    std::vector<glm::vec3> vertices = {
        {1,1,1}, {1,1,1}, {1,1,1}
    };

    Mesh mesh(triangle, vertices);

    auto bounds = mesh.m_meshBounds;

    REQUIRE(bounds.getMinBoundsPos() == glm::vec3(1));
    REQUIRE(bounds.getMaxBoundsPos() == glm::vec3(1));
}

//
// 🔹 NEGATIVE COORDINATES
//
TEST_CASE("Mesh bounds with negative values", "[mesh][bounds]") {
    std::vector<glm::vec3> vertices = {
        {-5,-5,-5}, {2,3,4}, {0,-1,10}
    };

    Mesh mesh(triangle, vertices);

    auto bounds = mesh.m_meshBounds;

    REQUIRE(bounds.getMinBoundsPos() == glm::vec3(-5,-5,-5));
    REQUIRE(bounds.getMaxBoundsPos() == glm::vec3(2,3,10));
}

//
// 🔹 VERY LARGE VALUES
//
TEST_CASE("Mesh handles large coordinates", "[mesh][bounds]") {
    std::vector<glm::vec3> vertices = {
        {1e6f, 1e6f, 1e6f},
        {-1e6f, -1e6f, -1e6f},
        {0,0,0}
    };

    Mesh mesh(triangle, vertices);

    auto bounds = mesh.m_meshBounds;

    REQUIRE(bounds.getMinBoundsPos() == glm::vec3(-1e6f));
    REQUIRE(bounds.getMaxBoundsPos() == glm::vec3(1e6f));
}

//
// 🔹 MULTIPLE POLYGONS
//
TEST_CASE("Mesh handles multiple faces", "[mesh]") {
    std::vector<std::vector<int>> polygons = {
        {0,1,2},
        {2,3,0}
    };

    std::vector<glm::vec3> vertices = {
        {0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}
    };

    Mesh mesh(polygons, vertices);

    auto* he = mesh.getHalfEdgeStructure();

    REQUIRE(he->m_faces.size() == 2);
    REQUIRE(he->m_vertices.size() == 4);
}

//
// 🔹 TRANSFORM DEFAULT
//
TEST_CASE("Mesh transform is identity", "[mesh][transform]") {
    std::vector<glm::vec3> vertices = {
        {0,0,0}, {1,0,0}, {0,1,0}
    };

    Mesh mesh(triangle, vertices);

    glm::mat4 identity(1.0f);
    REQUIRE(mesh.m_transform == identity);
}

//
// 🔹 REAL-TIME TRANSFORM POINTER
//
TEST_CASE("Mesh real-time transform pointer default null", "[mesh]") {
    std::vector<glm::vec3> vertices = {
        {0,0,0}, {1,0,0}, {0,1,0}
    };

    Mesh mesh(triangle, vertices);

    REQUIRE(mesh.m_realTimeTransform == nullptr);
}

//
// 🔹 SELECTION DEFAULTS
//
TEST_CASE("Mesh selection defaults", "[mesh]") {
    std::vector<glm::vec3> vertices = {
        {0,0,0}, {1,0,0}, {0,1,0}
    };

    Mesh mesh(triangle, vertices);

    REQUIRE(mesh.m_selected == false);
    REQUIRE(mesh.m_selectionIndex == -1);
}

//
// 🔹 BUILD SUCCESS FLAG (currently broken design)
//
TEST_CASE("Mesh isBuildSuccessful always true", "[mesh]") {
    std::vector<glm::vec3> vertices = {
        {0,0,0}, {1,0,0}, {0,1,0}
    };

    Mesh mesh(triangle, vertices);

    REQUIRE(mesh.isBuildSuccessful() == true);
}

//
// 🔹 BOUNDS AFTER MANUAL RECOMPUTE
//
TEST_CASE("Recomputing bounds is stable", "[mesh][bounds]") {
    std::vector<glm::vec3> vertices = {
        {0,0,0}, {2,2,2}, {1,1,1}
    };

    Mesh mesh(triangle, vertices);

    auto originalMin = mesh.m_meshBounds.getMinBoundsPos();
    auto originalMax = mesh.m_meshBounds.getMaxBoundsPos();

    mesh.calculateMeshBounds();

    REQUIRE(mesh.m_meshBounds.getMinBoundsPos() == originalMin);
    REQUIRE(mesh.m_meshBounds.getMaxBoundsPos() == originalMax);
}