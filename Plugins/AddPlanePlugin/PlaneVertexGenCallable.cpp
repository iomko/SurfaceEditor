#include "PlaneVertexGenCallable.h"
#include "../../src/Callables/CallableRegistry.h"
#include "../../src/Renderer/MaterialRegistry.h"

static AutoRegisterCallable<PlaneVertexGenCallable> reg("PLANE_VERTEX_GEN_CALLABLE");

void PlaneVertexGenCallable::invoke(const PlaneParams &input, MeshParams &output) 
{
    float planeSize = input.m_size;
    int planeSubidivisionLevel = input.m_subdivisionLevel;
    glm::vec3 position = input.m_position;

    std::vector<std::vector<int>> planeIndices;
    std::vector<glm::vec3> planeVertices;

    float squareSize = planeSize / planeSubidivisionLevel;

    for (int z = 0; z <= planeSubidivisionLevel; ++z)
    {
        for (int x = 0; x <= planeSubidivisionLevel; ++x)
        {
            glm::vec3 vertex{(-(planeSize / 2)) + (x * squareSize), 0.0f, (-(planeSize / 2)) + (z * squareSize)};
            vertex += position;

            planeVertices.push_back(vertex);

            if (x != planeSubidivisionLevel && z != planeSubidivisionLevel)
            {
                int firstVertexIndex = (z * (planeSubidivisionLevel + 1)) + x;
                int secondVertexIndex = ((z + 1) * (planeSubidivisionLevel + 1)) + x;
                int thirdVertexIndex = (z * (planeSubidivisionLevel + 1)) + x + 1;
                int fourthVertexIndex = ((z + 1) * (planeSubidivisionLevel + 1)) + x + 1;

                planeIndices.push_back({firstVertexIndex, secondVertexIndex, thirdVertexIndex});
                planeIndices.push_back({fourthVertexIndex, thirdVertexIndex, secondVertexIndex});
            }
        }
    }

    // get proper material
    Mesh *mesh = new Mesh(planeIndices, planeVertices);
    mesh->m_transform[3].x = position.x;
    mesh->m_transform[3].y = position.y;
    mesh->m_transform[3].z = position.z;
    
    output.m_mesh = mesh;
}
