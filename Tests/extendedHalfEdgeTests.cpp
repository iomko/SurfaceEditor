#include "../src/Structures/ExtendedHalfEdge.h"
#include <catch2/catch_test_macros.hpp>

#include <set>
#include <random>


//EXTENDED HALF-EDGE TESTS

TEST_CASE("Build simple triangle mesh", "[extendedHalfEdge]") {
    ExtendedHalfEdgeMesh mesh;

    std::vector<std::vector<int>> polygons = {
        {0,1,2}
    };

    std::vector<glm::vec3> vertices = {
        {0,0,0}, {1,0,0}, {0,1,0}
    };

    mesh.build(polygons, vertices);

    REQUIRE(mesh.m_faces.size() == 1);
    REQUIRE(mesh.m_edges.size() == 3);
    REQUIRE(mesh.m_vertices.size() == 3);
    REQUIRE(mesh.m_halfEdges.size() == 3);
}

TEST_CASE("Face forms closed half-edge loop", "[extendedHalfEdge]") {
    ExtendedHalfEdgeMesh mesh;

    mesh.build({{0,1,2}}, {
        {0,0,0},{1,0,0},{0,1,0}
    });

    auto* face = mesh.m_faces[0];
    auto* start = face->m_halfEdge;

    auto* he = start;
    int count = 0;

    do {
        REQUIRE(he->m_next != nullptr);
        REQUIRE(he->m_previous != nullptr);

        he = he->m_next;
        count++;

    } while (he != start);

    REQUIRE(count == 3);
}

TEST_CASE("Twin edges are assigned correctly", "[extendedHalfEdge]") {
    ExtendedHalfEdgeMesh mesh;

    mesh.build({
        {0,1,2},
        {2,1,3}
    }, {
        {0,0,0},{1,0,0},{0,1,0},{1,1,0}
    });

    int twinCount = 0;

    for (auto* he : mesh.m_halfEdges) {
        if (he->m_twin != nullptr) {
            twinCount++;
            REQUIRE(he->m_twin->m_twin == he);
        }
    }

    REQUIRE(twinCount > 0);
}

TEST_CASE("Vertices have outgoing half-edge", "[extendedHalfEdge]") {
    ExtendedHalfEdgeMesh mesh;

    mesh.build({{0,1,2}}, {
        {0,0,0},{1,0,0},{0,1,0}
    });

    for (auto* v : mesh.m_vertices) {
        REQUIRE(v->m_halfEdge != nullptr);
    }
}

TEST_CASE("Edges connect correct vertices", "[extendedHalfEdge]") {
    ExtendedHalfEdgeMesh mesh;

    mesh.build({{0,1,2}}, {
        {0,0,0},{1,0,0},{0,1,0}
    });

    for (auto* edge : mesh.m_edges) {
        REQUIRE(edge->m_firstVertex != nullptr);
        REQUIRE(edge->m_secondVertex != nullptr);
        REQUIRE(edge->m_halfEdge != nullptr);
    }
}

TEST_CASE("Face vertex iteration works", "[extendedHalfEdge]") {
    ExtendedHalfEdgeMesh mesh;

    mesh.build({{0,1,2}}, {
        {0,0,0},{1,0,0},{0,1,0}
    });

    auto* face = mesh.m_faces[0];

    int count = 0;
    for (auto it = face->faceVertexBegin(); it != face->faceVertexEnd(); ++it) {
        count++;
    }

    REQUIRE(count == 3);
}

TEST_CASE("Face half-edge iteration works", "[extendedHalfEdge]") {
    ExtendedHalfEdgeMesh mesh;

    mesh.build({{0,1,2}}, {
        {0,0,0},{1,0,0},{0,1,0}
    });

    auto* face = mesh.m_faces[0];

    int count = 0;
    for (auto it = face->faceHalfEdgeBegin(); it != face->faceHalfEdgeEnd(); ++it) {
        count++;
    }

    REQUIRE(count == 3);
}

TEST_CASE("Graph builds correctly", "[extendedHalfEdge][graph]") {
    ExtendedHalfEdgeMesh mesh;

    mesh.build({{0,1,2}}, {
        {0,0,0},{1,0,0},{0,1,0}
    });

    mesh.ensureGraphBuilt();

    auto* face = mesh.m_faces[0];

    REQUIRE(face->m_graphEdges.size() == 3);

    for (auto* v : mesh.m_vertices) {
        REQUIRE(v->m_graphEdges.size() > 0);
    }
}

TEST_CASE("Floating face creates valid structure", "[extendedHalfEdge][floating]") {
    ExtendedHalfEdgeMesh mesh;

    std::vector<glm::vec3> verts = {
        {0,0,0},{1,0,0},{1,1,0},{0,1,0}
    };

    auto* face = mesh.addFloatingFace(verts);

    REQUIRE(face != nullptr);
    REQUIRE(mesh.m_faces.size() == 1);
    REQUIRE(mesh.m_edges.size() == 4);
}

TEST_CASE("Connecting edges updates topology", "[extendedHalfEdge][connect]") {
    ExtendedHalfEdgeMesh mesh;

    auto* face1 = mesh.addFloatingFace({
        {0,0,0},{1,0,0},{1,1,0}
    });

    auto* face2 = mesh.addFloatingFace({
        {1,1,0},{1,0,0},{2,0,0}
    });

    auto* edge1 = mesh.m_edges[0];
    auto* edge2 = mesh.m_edges[1];

    mesh.connectEdges(edge1, edge2);

    // sanity check (no crash + still valid)
    REQUIRE(mesh.m_edges.size() > 0);
}

TEST_CASE("Large mesh build stress test", "[extendedHalfEdge][stress]") {
    ExtendedHalfEdgeMesh mesh;

    std::vector<std::vector<int>> polys;
    std::vector<glm::vec3> verts;

    for (int i = 0; i < 100; ++i) {
        verts.push_back({float(i),0,0});
        verts.push_back({float(i),1,0});
        verts.push_back({float(i+1),0,0});

        polys.push_back({i*3, i*3+1, i*3+2});
    }

    mesh.build(polys, verts);

    REQUIRE(mesh.m_faces.size() == 100);
}