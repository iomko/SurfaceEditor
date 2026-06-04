#include "../src/Structures/ExtendedHalfEdge.h"
#include <catch2/catch_test_macros.hpp>

#include <set>
#include <random>

extern size_t get_active_allocations();
extern void reset_allocation_counters();
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
TEST_CASE("Floating face graph edges are not duplicated", "[extendedHalfEdge][bug_check]") {
    ExtendedHalfEdgeMesh mesh;

    // Create a 3-vertex floating face
    auto* face = mesh.addFloatingFace({
        {0,0,0},{1,0,0},{1,1,0}
    });

    // A triangle should only have 3 graph edges associated with it.
    // If this fails and says 9, you have a nested loop bug in buildGraphForFace!
    REQUIRE(face->m_graphEdges.size() == 3);

    // Each vertex in a single standalone triangle should only have 1 graph edge pointing to this face
    for (auto* graphEdge : face->m_graphEdges) {
        REQUIRE(graphEdge->vertex->m_graphEdges.size() == 1);
    }
}
TEST_CASE("Graph is built lazily and not duplicated on subsequent calls", "[extendedHalfEdge][lazy]") {
    ExtendedHalfEdgeMesh mesh;

    mesh.build({{0,1,2}}, {
        {0,0,0},{1,0,0},{0,1,0}
    });

    // 1. Ensure graph is NOT built upon initialization
    REQUIRE(mesh.isGraphBuilt() == false);
    REQUIRE(mesh.m_faces[0]->m_graphEdges.empty());

    // 2. Build the graph
    mesh.ensureGraphBuilt();
    REQUIRE(mesh.isGraphBuilt() == true);
    size_t graphEdgeCount = mesh.m_faces[0]->m_graphEdges.size();
    REQUIRE(graphEdgeCount == 3);

    // 3. Call it again to ensure it bails out early and doesn't duplicate data
    mesh.ensureGraphBuilt();
    REQUIRE(mesh.m_faces[0]->m_graphEdges.size() == graphEdgeCount);
}
TEST_CASE("Floating face correctly connects previous and next half-edges", "[extendedHalfEdge][floating]") {
    ExtendedHalfEdgeMesh mesh;

    auto* face = mesh.addFloatingFace({
        {0,0,0},{1,0,0},{1,1,0},{0,1,0} // A Quad
    });

    auto* start = face->m_halfEdge;
    auto* current = start;
    int forwardCount = 0;

    // Test forward traversal (m_next)
    do {
        REQUIRE(current->m_next != nullptr);
        // Verify bidirectional integrity
        REQUIRE(current->m_next->m_previous == current);
        
        current = current->m_next;
        forwardCount++;
    } while (current != start && forwardCount < 10);

    REQUIRE(forwardCount == 4);
}
TEST_CASE("Connecting edges correctly reduces vertex and edge counts", "[extendedHalfEdge][connect]") {
    ExtendedHalfEdgeMesh mesh;

    auto* face1 = mesh.addFloatingFace({ {0,0,0}, {1,0,0}, {0,1,0} });
    auto* face2 = mesh.addFloatingFace({ {0,1,0}, {1,0,0}, {1,1,0} });

    // Pre-connection counts: 2 isolated triangles = 6 vertices, 6 edges
    REQUIRE(mesh.m_vertices.size() == 6);
    REQUIRE(mesh.m_edges.size() == 6);

    // Find the shared edges.
    // Face 1 edge going from {1,0,0} to {0,1,0}
    // Face 2 edge going from {0,1,0} to {1,0,0}
    ExtendedEdge* edge1 = nullptr;
    ExtendedEdge* edge2 = nullptr;

    for(auto* e : mesh.m_edges) {
        if(e->m_firstVertex->m_position == glm::vec3(1,0,0) && e->m_secondVertex->m_position == glm::vec3(0,1,0)) edge1 = e;
        if(e->m_firstVertex->m_position == glm::vec3(0,1,0) && e->m_secondVertex->m_position == glm::vec3(1,0,0)) edge2 = e;
    }

    REQUIRE(edge1 != nullptr);
    REQUIRE(edge2 != nullptr);

    mesh.connectEdges(edge1, edge2);

    // Post-connection counts: 
    // Vertices merged at the shared edge -> 4 total vertices
    // 2 edges merged into 1 shared twin edge -> 5 total edges
    REQUIRE(mesh.m_vertices.size() == 4);
    REQUIRE(mesh.m_edges.size() == 5);
    
    // Verify the remaining merged edge correctly acts as a twin bridge
    REQUIRE(edge2->m_halfEdge->m_twin != nullptr);
}
TEST_CASE("Connecting incompatible edges safely aborts", "[extendedHalfEdge][connect]") {
    ExtendedHalfEdgeMesh mesh;

    mesh.addFloatingFace({ {0,0,0}, {1,0,0}, {0,1,0} });
    mesh.addFloatingFace({ {10,10,0}, {11,10,0}, {10,11,0} }); // Completely separate

    size_t initialVerts = mesh.m_vertices.size();
    size_t initialEdges = mesh.m_edges.size();

    // Try to connect two edges that are nowhere near each other
    mesh.connectEdges(mesh.m_edges[0], mesh.m_edges[4]);

    // Topology should remain completely untouched
    REQUIRE(mesh.m_vertices.size() == initialVerts);
    REQUIRE(mesh.m_edges.size() == initialEdges);
}
TEST_CASE("ExtendedHalfEdgeMesh destroys all graph allocations without leaking", "[extendedHalfEdge][leak]") {
    // 1. Snapshot active allocations before creating the mesh
    reset_allocation_counters();
    size_t initial_allocations = get_active_allocations();

    {
        ExtendedHalfEdgeMesh mesh;

        // Add a floating face (triggers buildGraphForFace)
        mesh.addFloatingFace({
            {0,0,0}, {1,0,0}, {1,1,0}
        });

        // Ensure bulk graph is built too
        mesh.ensureGraphBuilt();
        
        // Quick sanity check that we actually allocated things on the heap
        REQUIRE(get_active_allocations() > initial_allocations);
    } 
    // <-- Mesh goes out of scope here and its destructor (~ExtendedHalfEdgeMesh) runs

    // 2. Snapshot allocations after destruction
    size_t final_allocations = get_active_allocations();

    // IF THIS FAILS: Your mesh destructor is leaking GraphEdge elements or vertices/edges!
    REQUIRE(final_allocations == initial_allocations);
}

TEST_CASE("clearGraph resets graph data and allows rebuild", "[extendedHalfEdge][graph][clear]") {
    ExtendedHalfEdgeMesh mesh;

    mesh.build({{0,1,2}}, {{0,0,0}, {1,0,0}, {0,1,0}});
    mesh.ensureGraphBuilt();

    REQUIRE(mesh.isGraphBuilt());
    REQUIRE(mesh.m_faces[0]->m_graphEdges.size() == 3);

    mesh.clearGraph();

    REQUIRE_FALSE(mesh.isGraphBuilt());
    REQUIRE(mesh.m_faces[0]->m_graphEdges.empty());

    mesh.ensureGraphBuilt();

    REQUIRE(mesh.isGraphBuilt());
    REQUIRE(mesh.m_faces[0]->m_graphEdges.size() == 3);
}
