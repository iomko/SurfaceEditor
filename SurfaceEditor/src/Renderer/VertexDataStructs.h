#pragma once
#include <glm/vec3.hpp>
#include <array>

struct AABBVertex {
    glm::vec3 position;
    glm::vec3 color;
};

struct LineVertex {
    glm::vec3 point;
    float isHighlighted;
};

struct MeshVertex
{
    glm::vec3 position;
    glm::vec3 color;
};

struct MeshPoint
{
	glm::vec3 position;
    float isHighlighted = 0.0f;
};

struct MeshVert
{
    glm::vec3 position;
    glm::vec3 normal;
    float isHighlited = 0.0f;
};

struct HalfEdgeTriangle
{
    std::array<glm::vec3, 3> positions;
};