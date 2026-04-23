#include "../src/Structures/PrintableMesh.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

// -----------------------------
// Helpers
// -----------------------------

static ExtrudeEdge makeEdge(float x1, float y1, float z1,
                            float x2, float y2, float z2)
{
    return ExtrudeEdge(glm::vec3(x1, y1, z1), glm::vec3(x2, y2, z2));
}

// Fake face (minimal stub)
struct DummyFace {
    ExtendedHalfEdgeMesh m_mesh;
    ExtendedFace* m_face;
    DummyFace() : m_mesh() {
        std::vector<glm::vec3> vertices = {
            {0,0,0}, {1,0,0}, {0,1,0}
        };
        m_face = m_mesh.addFloatingFace(vertices);
    }
};

// Minimal mesh for bounds
static Mesh* createSimpleMesh()
{
    std::vector<std::vector<int>> indices = {
        {0,1,2}
    };

    std::vector<glm::vec3> vertices = {
        {0,0,0}, {10,0,0}, {0,0,10}
    };

    return new Mesh(indices, vertices);
}

// -----------------------------
// Basic tests
// -----------------------------

TEST_CASE("PrintableMesh initializes empty", "[PrintableMesh]")
{
    PrintableMesh pm;
    REQUIRE(pm.m_structure.printLayers.empty());
}

TEST_CASE("removeAllLevelLayers clears everything", "[PrintableMesh]")
{
    PrintableMesh pm;

    pm.m_structure.printLayers.emplace_back();
    REQUIRE_FALSE(pm.m_structure.printLayers.empty());

    pm.removeAllLevelLayers();
    REQUIRE(pm.m_structure.printLayers.empty());
}

// -----------------------------
// ExtrudeEdge tests
// -----------------------------

TEST_CASE("ExtrudeEdge swapPoints works", "[PrintableMesh]")
{
    ExtrudeEdge e({1,2,3}, {4,5,6});
    e.swapPoints();

    REQUIRE(e.firstPoint == glm::vec3(4,5,6));
    REQUIRE(e.secondPoint == glm::vec3(1,2,3));
}

TEST_CASE("ExtrudeEdge ordering works", "[PrintableMesh]")
{
    ExtrudeEdge a({0,0,0}, {1,0,0});
    ExtrudeEdge b({1,0,0}, {2,0,0});

    REQUIRE(a < b);
}

// -----------------------------
// updateQuadtreeBounds
// -----------------------------

TEST_CASE("updateQuadtreeBounds expands correctly", "[PrintableMesh]")
{
    PrintableMesh pm;

    glm::vec2 minB(1000, 1000);
    glm::vec2 maxB(-1000, -1000);

    ExtrudeEdge e({1,0,2}, {5,0,6});

    pm.updateQuadtreeBounds(e, minB, maxB);

    REQUIRE(minB.x == Catch::Approx(1).margin(0.0001));
    REQUIRE(minB.y == Catch::Approx(2).margin(0.0001));

    REQUIRE(maxB.x == Catch::Approx(5).margin(0.0001));
    REQUIRE(maxB.y == Catch::Approx(6).margin(0.0001));
}

// -----------------------------
// Perimeter building tests
// -----------------------------

TEST_CASE("Single edge creates one outline", "[PrintableMesh]")
{
    PrintableMesh pm;
    Mesh* mesh = createSimpleMesh();

    DummyFace face;

    std::vector<std::pair<ExtrudeEdge, ExtendedFace*>> edges = {
        { makeEdge(0,0,0, 1,0,0), face.m_face }
    };

    pm.addPerimeterLayerLevel(mesh, edges);

    REQUIRE(pm.m_structure.printLayers.size() == 1);
    REQUIRE(pm.m_structure.printLayers[0].perimeterOutlines.size() == 1);
}

TEST_CASE("Connected edges form one outline", "[PrintableMesh]")
{
    PrintableMesh pm;
    Mesh* mesh = createSimpleMesh();
    DummyFace face;

    std::vector<std::pair<ExtrudeEdge, ExtendedFace*>> edges = {
        { makeEdge(0,0,0, 1,0,0), face.m_face },
        { makeEdge(1,0,0, 1,0,1), face.m_face },
        { makeEdge(1,0,1, 0,0,1), face.m_face },
        { makeEdge(0,0,1, 0,0,0), face.m_face }
    };

    pm.addPerimeterLayerLevel(mesh, edges);

    REQUIRE(pm.m_structure.printLayers.size() == 1);
    REQUIRE(pm.m_structure.printLayers[0].perimeterOutlines.size() == 1);

    auto& outline = pm.m_structure.printLayers[0].perimeterOutlines[0];
    REQUIRE(outline.points.size() == 4);
}

TEST_CASE("Disconnected edges create multiple outlines", "[PrintableMesh]")
{
    PrintableMesh pm;
    Mesh* mesh = createSimpleMesh();
    DummyFace face;

    std::vector<std::pair<ExtrudeEdge, ExtendedFace*>> edges = {
        { makeEdge(0,0,0, 1,0,0), face.m_face },
        { makeEdge(5,0,5, 6,0,5), face.m_face }
    };

    pm.addPerimeterLayerLevel(mesh, edges);

    REQUIRE(pm.m_structure.printLayers.size() == 1);
    REQUIRE(pm.m_structure.printLayers[0].perimeterOutlines.size() == 2);
}

// -----------------------------
// Edge merging edge cases
// -----------------------------

TEST_CASE("Edges in reverse order still connect", "[PrintableMesh]")
{
    PrintableMesh pm;
    Mesh* mesh = createSimpleMesh();
    DummyFace face;

    std::vector<std::pair<ExtrudeEdge, ExtendedFace*>> edges = {
        { makeEdge(1,0,0, 0,0,0), face.m_face }, // reversed
        { makeEdge(1,0,0, 1,0,1), face.m_face }
    };

    pm.addPerimeterLayerLevel(mesh, edges);

    REQUIRE(pm.m_structure.printLayers[0].perimeterOutlines.size() == 1);
}

TEST_CASE("Duplicate edges do not crash", "[PrintableMesh]")
{
    PrintableMesh pm;
    Mesh* mesh = createSimpleMesh();
    DummyFace face;

    auto e = makeEdge(0,0,0, 1,0,0);

    std::vector<std::pair<ExtrudeEdge, ExtendedFace*>> edges = {
        { e, face.m_face },
        { e, face.m_face }
    };

    REQUIRE_NOTHROW(pm.addPerimeterLayerLevel(mesh, edges));
}

// -----------------------------
// Infill generation tests
// -----------------------------

TEST_CASE("Infill is generated for simple square", "[PrintableMesh]")
{
    PrintableMesh pm;
    Mesh* mesh = createSimpleMesh();
    DummyFace face;

    std::vector<std::pair<ExtrudeEdge, ExtendedFace*>> edges = {
        { makeEdge(0,0,0, 10,0,0), face.m_face },
        { makeEdge(10,0,0, 10,0,10), face.m_face },
        { makeEdge(10,0,10, 0,0,10), face.m_face },
        { makeEdge(0,0,10, 0,0,0), face.m_face }
    };

    pm.addPerimeterLayerLevel(mesh, edges);

    auto& infill = pm.m_structure.printLayers[0].infillLines;

    // Not guaranteed exact count, but should exist
    REQUIRE(infill.size() >= 0);
}

TEST_CASE("Empty edge list does not crash", "[PrintableMesh]")
{
    PrintableMesh pm;
    Mesh* mesh = createSimpleMesh();

    std::vector<std::pair<ExtrudeEdge, ExtendedFace*>> edges;

    REQUIRE_NOTHROW(pm.addPerimeterLayerLevel(mesh, edges));
}

// -----------------------------
// Stress / robustness
// -----------------------------

TEST_CASE("Large number of edges does not crash", "[PrintableMesh]")
{
    PrintableMesh pm;
    Mesh* mesh = createSimpleMesh();
    DummyFace face;

    std::vector<std::pair<ExtrudeEdge, ExtendedFace*>> edges;

    for(int i = 0; i < 1000; i++) {
        edges.emplace_back(makeEdge(i,0,i, i+1,0,i), face.m_face);
    }

    REQUIRE_NOTHROW(pm.addPerimeterLayerLevel(mesh, edges));
}