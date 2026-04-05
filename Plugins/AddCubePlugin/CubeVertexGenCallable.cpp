#include "CubeVertexGenCallable.h"
#include "../../src/Renderer/MaterialRegistry.h"

#include "../../src/Callables/CallableRegistry.h"

static AutoRegisterCallable<CubeVertexGenCallable> reg("CUBE_VERTEX_GEN_CALLABLE");

void CubeVertexGenCallable::invoke(const CubeParams &input, MeshParams &output)
{
    float cubeSize = input.m_size;
    int subdivision = input.m_subdivisionLevel;
    glm::vec3 position = input.m_position;

    float squareSize = cubeSize / subdivision;

    std::vector<std::vector<int>> cubeIndices;
    std::vector<glm::vec3> cubeVertices;

    for (int face = 0; face < 6; ++face)
    {
        int vertexOffset = cubeVertices.size();
        printf("Size: %d\n", vertexOffset);
        for (int row = 0; row <= subdivision; ++row)
        {
            for (int col = 0; col <= subdivision; ++col)
            {
                glm::vec3 v{0.0f};

                // Map grid coordinates [-cubeSize/2, +cubeSize/2]
                float u = (-cubeSize / 2.0f) + col * squareSize;
                float w = (-cubeSize / 2.0f) + row * squareSize;

                switch (face)
                {
                case 0: // bottom (-Y)
                    v = {u, -cubeSize / 2.0f, w};
                    break;
                case 1: // top (+Y)
                    v = {u, +cubeSize / 2.0f, w};
                    break;
                case 2: // front (+Z)
                    v = {u, w, +cubeSize / 2.0f};
                    break;
                case 3: // back (-Z)
                    v = {u, w, -cubeSize / 2.0f};
                    break;
                case 4: // left (-X)
                    v = {-cubeSize / 2.0f, w, u};
                    break;
                case 5: // right (+X)
                    v = {+cubeSize / 2.0f, w, u};
                    break;
                }

                // Apply cube position offset
                v += position;
                cubeVertices.push_back(v);

                if (col != subdivision && row != subdivision)
                {
                    int a = vertexOffset + row * (subdivision + 1) + col;           // top-left
                    int b = vertexOffset + row * (subdivision + 1) + col + 1;       // top-right
                    int c = vertexOffset + (row + 1) * (subdivision + 1) + col;     // bottom-left
                    int d = vertexOffset + (row + 1) * (subdivision + 1) + col + 1; // bottom-right

                    if (face == 2 || face == 4 || face == 0)
                    {
                        cubeIndices.push_back({a, b, c});
                        cubeIndices.push_back({c, b, d});
                    }
                    else
                    {
                        cubeIndices.push_back({a, c, b});
                        cubeIndices.push_back({c, d, b});
                    }
                }
            }
        }
    }

    Mesh *mesh = new Mesh(cubeIndices, cubeVertices);
    mesh->m_transform[3].x = position.x;
    mesh->m_transform[3].y = position.y;
    mesh->m_transform[3].z = position.z;
    
    output.m_mesh = mesh;
}