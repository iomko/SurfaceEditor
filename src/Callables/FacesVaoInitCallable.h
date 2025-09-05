
#include "../Params/OperationParams.h"
#include "Callables/Callable.h"
#include "../Utils/GeometryUtils.h"
#include "../Renderer/Renderer.h"

class FacesVaoInitCallable : public Callable<FaceParams, void>
{
public:
	void invoke(const FaceParams& input) override
	{
        Mesh* mesh = input.mesh;
        std::vector<ExtendedFace*>& faces = *(input.faces);
    
		Material* material = mesh->m_defaultMaterial;

		RendererStageData::MatVertsMap& materialVertsMap = Renderer::s_stageData.meshMatsMap[mesh];

		std::vector<RendererStageData::MeshVertex>& materialVaoVertices = materialVertsMap[material];
		std::vector<RendererStageData::LineVertex>& meshLinesVaoVector = Renderer::s_stageData.meshLinesMap[mesh];
        
        std::vector<FaceTriangle>& halfEdgeFaceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles[material];


		//---FOR_FACES---
        for(ExtendedFace* face : faces) {
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


            for(ExtendedFace* face : faces) {

                for(auto it = face->faceHalfEdgeBegin(); it != face->faceHalfEdgeEnd(); ++it) 
                {
                    ExtendedHalfEdge* halfEdge = &(*it);
                    ExtendedEdge* edge = (*it).m_edge;

                    ExtendedVertex* edgeFirstVertex = edge->m_firstVertex;
                    ExtendedVertex* edgeSecondVertex = edge->m_secondVertex;


                    glm::vec3 normal = utils::geometry::computePolygonNormal(halfEdge->m_face);
                    
                    if (halfEdge->m_twin != nullptr)
                    {
                        glm::vec3 twinNormal = utils::geometry::computePolygonNormal(halfEdge->m_twin->m_face);
                        normal = glm::normalize(normal + twinNormal); // Average and normalize
                    }

                        
                    meshLinesVaoVector.emplace_back(edgeFirstVertex->m_position, false);
                    meshLinesVaoVector.emplace_back(edgeSecondVertex->m_position, false);


                    //---ADD_INFO_INTO_EDGE---
                    int edgeLineIndex = meshLinesVaoVector.size() - 2;
                    edge->m_EdgeLineIndex = edgeLineIndex;

                }

            }
            /*
            //---FOR_LINES---
            //
            for(auto it = face->faceHalfEdgeBegin(); it != face->faceHalfEdgeEnd(); ++it) 
            {
                ExtendedHalfEdge* halfEdge = &(*it);
                ExtendedEdge* edge = (*it).m_edge;

                ExtendedVertex* edgeFirstVertex = edge->m_firstVertex;
                ExtendedVertex* edgeSecondVertex = edge->m_secondVertex;


                glm::vec3 normal = utils::geometry::computePolygonNormal(halfEdge->m_face);
                
                if (halfEdge->m_twin != nullptr)
                {
                    glm::vec3 twinNormal = utils::geometry::computePolygonNormal(halfEdge->m_twin->m_face);
                    normal = glm::normalize(normal + twinNormal); // Average and normalize
                }

                    
                meshLinesVaoVector.emplace_back(edgeFirstVertex->m_position + (normal * 0.001f), false);
                meshLinesVaoVector.emplace_back(edgeSecondVertex->m_position + (normal * 0.001f), false);

                meshLinesVaoVector.emplace_back(edgeFirstVertex->m_position + (-normal * 0.001f), false);
                meshLinesVaoVector.emplace_back(edgeSecondVertex->m_position + (-normal * 0.001f), false);

                //---ADD_INFO_INTO_EDGE---
                int edgeLineIndex = meshLinesVaoVector.size() - 4;
                edge->m_EdgeLineIndex = edgeLineIndex;

            }
            */
        }
    }

};


