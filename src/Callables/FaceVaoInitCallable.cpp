#include "FaceVaoInitCallable.h"
#include "CallableRegistry.h"

#include "../Utils/GeometryUtils.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/MaterialRegistry.h"

static AutoRegisterCallable<FaceVaoInitCallable> reg;

void FaceVaoInitCallable::invoke(const SingleFaceParams& input)
	{
        Mesh* inputMesh = input.mesh;
        Material* defaultMeshMaterial = MaterialRegistry::getMaterial("defaultMeshMaterial");
        ExtendedFace* inputFace = input.face;
        glm::vec3 faceNormal = utils::geometry::computePolygonNormal(inputFace);
        std::vector<FaceTriangle>& faceTriangles = inputMesh->m_halfEdgeStructure->m_faceTriangles[defaultMeshMaterial];


        if (auto opt = inputMesh->bufferLayout.getTriangleBufferStorage(defaultMeshMaterial)) {
            TriangleBufferStorage& triangleBufferStorage = opt->get();
            std::vector<BufferStorageDataType::TriangleVertex>& triangleBufferVertices = triangleBufferStorage.data.vertices;


            //Triangulation
            std::vector<glm::vec3> faceVertices;
            for (auto it = inputFace->faceVertexBegin(); it != inputFace->faceVertexEnd(); ++it) {
                glm::vec3 vertexPosition = (*it).m_position;
                faceVertices.emplace_back(vertexPosition);
            }
            glm::vec3 firstVertex = faceVertices.at(0);
            faceVertices.emplace_back(firstVertex);

            std::vector<glm::vec3> triangulatedVertices = utils::geometry::triangulatePolygon(faceVertices);

            //Update ExtendedFace info
            inputFace->material = defaultMeshMaterial;
            for (int i = 0, indexInFace = 0; i < triangulatedVertices.size(); i += 3, ++indexInFace)
            {
                triangleBufferVertices.emplace_back(triangulatedVertices.at(i), faceNormal, false);
                triangleBufferVertices.emplace_back(triangulatedVertices.at(i + 1), faceNormal, false);
                triangleBufferVertices.emplace_back(triangulatedVertices.at(i + 2), faceNormal, false);

                FaceTriangle faceTriangle;
                faceTriangle.face = inputFace;
                faceTriangle.indexInVAO = triangleBufferVertices.size() - 3;
                faceTriangle.indexInFace = indexInFace;

                faceTriangles.emplace_back(faceTriangle);

                int faceTriangleIndex = faceTriangles.size() - 1;
                inputFace->faceTriangleIndices.emplace_back(faceTriangleIndex);
            }

        }

    }