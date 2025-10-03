#pragma once

#include "../Params/OperationParams.h"
#include "Callables/Callable.h"
#include "../Utils/GeometryUtils.h"
#include "../Renderer/Renderer.h"

class FaceVaoInitCallable : public Callable<NewFaceParams, void>
{
public:
	void invoke(const NewFaceParams& input) override
	{
        Mesh* inputMesh = input.mesh;    
		Material* material = inputMesh->m_defaultMaterial;
        ExtendedFace* inputFace = input.face;
        glm::vec3 faceNormal = utils::geometry::computePolygonNormal(inputFace);
        std::vector<FaceTriangle>& faceTriangles = inputMesh->m_halfEdgeStructure->m_faceTriangles[material];

        //Get Raw Mesh Buffer Data For Specified Mesh
        MeshBufferStorage* meshBufferStorage = Renderer::s_bufferRegistry.queryBuffer<MeshBufferStorage>();
        BufferData<RendererBuffersData::MeshVertex>* meshBufferData;
        meshBufferStorage->getBufferData(inputMesh, material, meshBufferData);
        std::vector<RendererBuffersData::MeshVertex>& meshBufferVertices = meshBufferData->vertices;
        

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
        inputFace->material = material;
        for (int i = 0, indexInFace = 0; i < triangulatedVertices.size(); i += 3, ++indexInFace)
        {
            meshBufferVertices.emplace_back(triangulatedVertices.at(i), faceNormal, false);
            meshBufferVertices.emplace_back(triangulatedVertices.at(i + 1), faceNormal, false);
            meshBufferVertices.emplace_back(triangulatedVertices.at(i + 2), faceNormal, false);

            FaceTriangle faceTriangle;
            faceTriangle.face = inputFace;
            faceTriangle.indexInVAO = meshBufferVertices.size() - 3;
            faceTriangle.indexInFace = indexInFace;

            faceTriangles.emplace_back(faceTriangle);

            int faceTriangleIndex = faceTriangles.size() - 1;
            inputFace->faceTriangleIndices.emplace_back(faceTriangleIndex);
        }
    }

};


