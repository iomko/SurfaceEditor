#pragma once

struct AABBVertex {
    glm::vec3 position;
    glm::vec3 color;
};

struct LineVertex {
    glm::vec3 position;
    float isHighlighted;
};

struct MeshPoint
{
	glm::vec3 position;
    float isHighlighted = 0.0f;
};

struct MeshVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    float isHighlited = 0.0f;
};