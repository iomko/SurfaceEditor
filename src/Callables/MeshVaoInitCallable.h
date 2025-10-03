#pragma once
#include "../Utils/GeometryUtils.h"
#include "Structures/ExtendedHalfEdge.h"
#include "Renderer/Renderer.h"

class MeshVaoInitCallable : public Callable<MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override
	{
		Mesh* mesh = input.m_mesh;

		std::vector<ExtendedFace*>& meshFaces = input.m_mesh->getHalfEdgeStructure()->m_faces;
		std::vector<ExtendedEdge*>& meshEdges = input.m_mesh->getHalfEdgeStructure()->m_edges;

		Material* material = mesh->m_defaultMaterial;

		std::vector<FaceTriangle>& halfEdgeFaceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles[material];

        //Register Mesh BufferStorage
        MeshBufferStorage* meshBufferStorage = Renderer::s_bufferRegistry.queryBuffer<MeshBufferStorage>();
        meshBufferStorage->registerBufferStorage(mesh, material);

        //Get Raw Mesh Buffer Data For Specified Mesh
        BufferData<RendererBuffersData::MeshVertex>* meshBufferData;
        meshBufferStorage->getBufferData(mesh, material, meshBufferData);
        std::vector<RendererBuffersData::MeshVertex>& meshBufferVertices = meshBufferData->vertices;
        
        //Register Line BufferStorage
        LineBufferStorage* lineBufferStorage = Renderer::s_bufferRegistry.queryBuffer<LineBufferStorage>();
        lineBufferStorage->registerBufferStorage(mesh);

        //Get Raw Line Buffer Data For Specified Mesh
        BufferData<RendererBuffersData::LineVertex>* lineBufferData;
        lineBufferStorage->getBufferData(mesh, lineBufferData);
        std::vector<RendererBuffersData::LineVertex>& lineBufferVertices = lineBufferData->vertices;


		for (ExtendedFace* meshFace : meshFaces)
		{
			meshFace->material = material;
			glm::vec3 faceNormal = utils::geometry::computePolygonNormal(meshFace);

            //Triangulation
			std::vector<glm::vec3> faceVertices;
			for (auto it = meshFace->faceVertexBegin(); it != meshFace->faceVertexEnd(); ++it) {
				glm::vec3 vertexPosition = (*it).m_position;
				faceVertices.emplace_back(vertexPosition);
			}
			glm::vec3 firstVertex = faceVertices.at(0);
			faceVertices.emplace_back(firstVertex);

			std::vector<glm::vec3> triangulatedVertices = utils::geometry::triangulatePolygon(faceVertices);

            //Update ExtendedFace info
			for (int i = 0, indexInFace; i < triangulatedVertices.size(); i += 3, ++indexInFace)
			{
				meshBufferVertices.emplace_back(triangulatedVertices.at(i), faceNormal, false);
				meshBufferVertices.emplace_back(triangulatedVertices.at(i + 1), faceNormal, false);
				meshBufferVertices.emplace_back(triangulatedVertices.at(i + 2), faceNormal, false);

				FaceTriangle faceTriangle;
				faceTriangle.face = meshFace;
				faceTriangle.indexInVAO = meshBufferVertices.size() - 3;
				faceTriangle.indexInFace = indexInFace;

				halfEdgeFaceTriangles.emplace_back(faceTriangle);

				int faceTriangleIndex = halfEdgeFaceTriangles.size() - 1;
				meshFace->faceTriangleIndices.emplace_back(faceTriangleIndex);
			}

		}

        //Update ExtendedEdge info
		for (ExtendedEdge* edge : meshEdges)
		{
			ExtendedVertex* firstVertex = edge->m_firstVertex;
			ExtendedVertex* secondVertex = edge->m_secondVertex;
			
			ExtendedHalfEdge* halfEdge = edge->m_halfEdge;
			glm::vec3 normal = utils::geometry::computePolygonNormal(halfEdge->m_face);

			if (halfEdge->m_twin != nullptr)
			{
				glm::vec3 twinNormal = utils::geometry::computePolygonNormal(halfEdge->m_twin->m_face);
				normal = glm::normalize(normal + twinNormal);
			}

			lineBufferVertices.emplace_back(firstVertex->m_position, false);
			lineBufferVertices.emplace_back(secondVertex->m_position, false);

			int edgeLineIndex = lineBufferVertices.size() - 2;
			edge->m_EdgeLineIndex = edgeLineIndex;
		}

        meshBufferStorage->updateBufferStorage(mesh, material);
        lineBufferStorage->updateBufferStorage(mesh);
	}
};
