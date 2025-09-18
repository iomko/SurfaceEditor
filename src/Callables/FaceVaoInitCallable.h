

#include "../Params/OperationParams.h"
#include "Callables/Callable.h"
#include "../Utils/GeometryUtils.h"
#include "../Renderer/Renderer.h"

class FaceVaoInitCallable : public Callable<NewFaceParams, void>
{
public:
	void invoke(const NewFaceParams& input) override
	{
        Mesh* mesh = input.mesh;
        ExtendedFace* face = input.face;
    
		Material* material = mesh->m_defaultMaterial;

        std::vector<FaceTriangle>& halfEdgeFaceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles[material];

        //potrebujeme ziskat materialVaoVertices
        MeshBufferStorage* meshBufferStorage = Renderer::s_bufferRegistry.queryBuffer<MeshBufferStorage>();
        BufferData<RendererBuffersData::MeshVertex>* meshBufferData;
        meshBufferStorage->getBufferData(mesh, material, meshBufferData);
        std::vector<RendererBuffersData::MeshVertex>& materialVaoVertices = meshBufferData->vertices;
         

         //---SET_FACE_MATERIAL---
        face->material = material;

        //---COLLECT_FACE_VERTICES---
        std::vector<glm::vec3> faceVertices;
        for (auto it = face->faceVertexBegin(); it != face->faceVertexEnd(); ++it) {
            glm::vec3 vertexPosition = (*it).m_position;
            faceVertices.emplace_back(vertexPosition);
        }
        glm::vec3 firstVertex = faceVertices.at(0);
        faceVertices.emplace_back(firstVertex);

        //---COMPUTE_FACE_NORMAL---
        glm::vec3 faceNormal = utils::geometry::computePolygonNormal(face);

        //---TRIANGULATION---
        std::vector<glm::vec3> triangulatedVertices = utils::geometry::triangulatePolygon(faceVertices);

        //---VAO_INITIALIZATION---
        //---FACE_TRIANGLES_INITIALIZAION---
        int indexInFace = 0;
        for (int i = 0; i < triangulatedVertices.size(); i += 3)
        {
            //---TRIANGLE---
            materialVaoVertices.emplace_back(triangulatedVertices.at(i), faceNormal, false);
            materialVaoVertices.emplace_back(triangulatedVertices.at(i + 1), faceNormal, false);
            materialVaoVertices.emplace_back(triangulatedVertices.at(i + 2), faceNormal, false);

            //---CREATE_FACE_TRIANGLE---
            FaceTriangle faceTriangle;
            faceTriangle.face = face;
            faceTriangle.indexInVAO = materialVaoVertices.size() - 3;
            faceTriangle.indexInFace = indexInFace;

            //---ADD_FACE_TRIANGLE_INTO_HALF_EDGE_STRUCTURE---
            halfEdgeFaceTriangles.emplace_back(faceTriangle);

            //---ADD_FACE_TRIANGLE_INDEX---
            int faceTriangleIndex = halfEdgeFaceTriangles.size() - 1;
            //add FaceTriangleIndex
            face->faceTriangleIndices.emplace_back(faceTriangleIndex);

            ++indexInFace;
        }

        
        //meshBufferStorage->updateBufferStorage(mesh, material);
    }

};


