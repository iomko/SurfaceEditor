#pragma once

import Renderer.Buffers;
#include "../AABBBoundingRegion.h"
#include <vector>
#include "VertexDataStructs.h"
#include "../Mesh.h"

struct RendererData {
    //POINT
    inline static VertexArrayObject Point_vao;
    inline static VertexBufferObject Point_vbo;

    //LINE
    inline static VertexArrayObject Line_vao;
    inline static VertexBufferObject Line_vbo;

    //MESH2
    inline static VertexArrayObject Mesh_vao;
    inline static VertexBufferObject Mesh_vbo;

    //AABB
    inline static VertexArrayObject AABB_vao;
    inline static VertexBufferObject AABB_vbo;
    inline static ElementBufferObject AABB_ebo;

    //SCREEN QUAD
    inline static unsigned int screenQuadVAO;
    inline static unsigned int screenQuadVBO;

    static const unsigned int maxBoxCount = 2000000;
    static const unsigned int maxVertexCount = 36 * maxBoxCount;
    static const unsigned int maxIndexCount = 24 * maxBoxCount;

    std::vector<AABBVertex> boundingBoxVertices;
};

class Renderer {
public:
    static void init();
    static void drawPoints(std::vector<MeshPoint>& points);
    static void drawMesh(std::vector<MeshVertex>& mesh);
    static void drawMeshDefault(const std::vector<MeshVert>& mesh);
    static void collectAABBdata(const AABBBoundingRegion& aabb, const glm::vec3& vertexColor);
    static void drawBoundingBoxes();
    static void drawLines(std::vector<LineVertex>& lines);
    static void drawLine(const glm::vec3& startPoint, const glm::vec3& endPoint);
    //temporary
    static void deleteAABBBuffer();
};
