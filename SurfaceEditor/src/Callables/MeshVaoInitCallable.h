#pragma once
#include "../Triangulator.h"
#include "../Utils/GeometryUtils.h"

class MeshVaoInitCallable : public Callable<MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override
	{
		Mesh* mesh = input.m_mesh;

		std::vector<HalfEdgeDS::Face*>& meshFaces = input.m_mesh->getHalfEdgeStructure()->m_faces;
		std::vector<HalfEdgeDS::Edge*>& meshEdges = input.m_mesh->getHalfEdgeStructure()->m_edges;

		SceneRes& res = ViewPortsHolderContext::m_viewPortsHolder->m_scene->m_res;

		Material* material = mesh->m_defaultMaterial;
		SceneRes::MaterialVaoMap& meshFacesVaoMap = res.meshData.meshFacesVaoMap[mesh];
		std::vector<LineVertex>& meshLinesVaoVector = res.meshData.meshLinesVaoMap[mesh];

		std::vector<MeshVertex>& materialVaoVertices = meshFacesVaoMap[material];
		std::vector<HalfEdgeDS::FaceTriangle>& halfEdgeFaceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles[material];

		//---FOR_FACES---
		for (HalfEdgeDS::Face* meshFace : meshFaces)
		{
			//---SET_FACE_MATERIAL---
			meshFace->material = material;

			//---COLLECT_FACE_VERTICES---
			std::vector<glm::vec3> faceVertices;
			for (auto it = meshFace->faceVertexBegin(); it != meshFace->faceVertexEnd(); ++it) {
				glm::vec3 vertexPosition = (*it).m_position;
				faceVertices.emplace_back(vertexPosition);
			}
			glm::vec3 firstVertex = faceVertices.at(0);
			faceVertices.emplace_back(firstVertex);

			//---COMPUTE_FACE_NORMAL---
			glm::vec3 faceNormal = utils::geometry::computePolygonNormal(meshFace);

			//---TRIANGULATION---
			std::vector<glm::vec3> triangulatedVertices = Triangulator::triangulatePolygon(faceVertices);

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
				HalfEdgeDS::FaceTriangle faceTriangle;
				faceTriangle.face = meshFace;
				faceTriangle.indexInVAO = materialVaoVertices.size() - 3;
				faceTriangle.indexInFace = indexInFace;

				//---ADD_FACE_TRIANGLE_INTO_HALF_EDGE_STRUCTURE---
				halfEdgeFaceTriangles.emplace_back(faceTriangle);

				//---ADD_FACE_TRIANGLE_INDEX---
				int faceTriangleIndex = halfEdgeFaceTriangles.size() - 1;
				//add FaceTriangleIndex
				meshFace->faceTriangleIndices.emplace_back(faceTriangleIndex);

				++indexInFace;
			}

		}

		//---FOR_LINES---
		for (HalfEdgeDS::Edge* edge : meshEdges)
		{
			HalfEdgeDS::Vertex* edgeFirstVertex = edge->m_firstVertex;
			HalfEdgeDS::Vertex* edgeSecondVertex = edge->m_secondVertex;

			

			//mozeme si pamatam v tomto vao, vrchny aj spodny edge

			//---CREATE_LINE_VAO_DATA---
			//---TOP_EDGE---
			HalfEdgeDS::HalfEdge* halfEdge = edge->m_halfEdge;

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

	}
};