#pragma once

#include "../Params/OperationParams.h"
#include "Callables/Callable.h"
#include "../Utils/GeometryUtils.h"
#include "../Renderer/Renderer.h"

class FacesVaoInitCallable : public Callable<FaceParams, void>
{
public:
	void invoke(const FaceParams& input) override
	{
        Mesh* inputMesh = input.mesh;
        std::vector<ExtendedFace*>& inputFaces = *(input.faces);
		Material* material = inputMesh->m_defaultMaterial;
        std::vector<FaceTriangle>& faceTriangles = inputMesh->m_halfEdgeStructure->m_faceTriangles[material];

        //Get Raw Mesh Buffer Data For Specified Mesh
        MeshBufferStorage* meshBufferStorage = Renderer::s_bufferRegistry.queryBuffer<MeshBufferStorage>();
        BufferData<RendererBuffersData::MeshVertex>* meshBufferData;
        meshBufferStorage->getBufferData(inputMesh, material, meshBufferData);
        std::vector<RendererBuffersData::MeshVertex>& meshBufferVertices = meshBufferData->vertices;
         
        for(ExtendedFace* face : inputFaces) {
            face->material = material;

            glm::vec3 faceNormal = utils::geometry::computePolygonNormal(face);

            //Triangulation
            std::vector<glm::vec3> faceVertices;
            for (auto it = face->faceVertexBegin(); it != face->faceVertexEnd(); ++it) {
                glm::vec3 vertexPosition = (*it).m_position;
                faceVertices.emplace_back(vertexPosition);
            }
            glm::vec3 firstVertex = faceVertices.at(0);
            faceVertices.emplace_back(firstVertex);

            std::vector<glm::vec3> triangulatedVertices = utils::geometry::triangulatePolygon(faceVertices);

            //Update ExtendedFace info
            int indexInFace = 0;
            for (int i = 0, indexInFace; i < triangulatedVertices.size(); i += 3, ++indexInFace)
            {
                meshBufferVertices.emplace_back(triangulatedVertices.at(i), faceNormal, false);
                meshBufferVertices.emplace_back(triangulatedVertices.at(i + 1), faceNormal, false);
                meshBufferVertices.emplace_back(triangulatedVertices.at(i + 2), faceNormal, false);

                FaceTriangle faceTriangle;
                faceTriangle.face = face;
                faceTriangle.indexInVAO = meshBufferVertices.size() - 3;
                faceTriangle.indexInFace = indexInFace;

                faceTriangles.emplace_back(faceTriangle);

                int faceTriangleIndex = faceTriangles.size() - 1;
                face->faceTriangleIndices.emplace_back(faceTriangleIndex);
            }

        }
        
        meshBufferStorage->updateBufferStorage(inputMesh, material);
    }

};


