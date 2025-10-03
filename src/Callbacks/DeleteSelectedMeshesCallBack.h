#pragma once
#include "Structures/ExtendedHalfEdge.h"

class DeleteSelectedMeshesCallBack : public Callback<>, public Observer
{
public:
	void execute() override
	{
		const SelectionHolder& selectionsHolder = ViewPortsHolderContext::s_selectionController->getHolder();
		const std::vector<Mesh*>& selectedMeshes = selectionsHolder.meshes;
		Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

		while(!selectedMeshes.empty())
		{
			Mesh* mesh = selectedMeshes.back();

			//---DELETE_FROM_OCTREES---
			scene->deleteMeshFromOctrees(mesh);
			//---DELETE_FROM_OCTREES---

			//---DELETE_FROM_HALFEDGE_STRUCTURE---
			deleteMeshVaoData(mesh);
			deleteMeshMeshData(mesh);
			//---DELETE_FROM_HALFEDGE_STRUCTURE---

			//---DELETE_FROM_SELECTIONS---
			ViewPortsHolderContext::s_selectionController->unregisterMesh(mesh);
			//---DELETE_FROM_SELECTIONS---
		}
	}

private:

	void deleteMeshVaoData(Mesh* mesh)
	{
		for (ExtendedFace* face : mesh->m_halfEdgeStructure->m_faces)
		{
			deleteFaceVaoData(mesh, face);
		}
	}
	
	void deleteMeshMeshData(Mesh* mesh)
	{
		while (!mesh->m_halfEdgeStructure->m_faces.empty())
		{
			ExtendedFace* face = mesh->m_halfEdgeStructure->m_faces.back();
			deleteFaceMeshData(mesh, face);
		}
	}

	void deleteFaceMeshData(Mesh* mesh, ExtendedFace* face)
	{
		ExtendedHalfEdgeMesh* halfEdgeMesh = mesh->m_halfEdgeStructure;
		//idem postupne cez vsetky vertices
		ExtendedHalfEdge* halfEdge = face->m_halfEdge;
		do {
			//do something with the vertex Vertex*
			//and the halfEdge HalfEdge*
			ExtendedVertex* vertex = halfEdge->m_vertex;

			ExtendedHalfEdge* nextHalfEdge = halfEdge->m_next;

			//potrebujem vymazat graphEdge z m_graphEdges

			//najskor z vertexu
			GraphEdge* foundGraphEdge = nullptr;
			for (GraphEdge* graphEdge : vertex->m_graphEdges)
			{
				if (graphEdge->face == face)
				{
					foundGraphEdge = graphEdge;
					break;
				}
			}

            //Delete GraphEdge
			if (foundGraphEdge->graphEdgeIndexInVertex != (vertex->m_graphEdges.size() - 1))
			{
				vertex->m_graphEdges.back()->graphEdgeIndexInVertex = foundGraphEdge->graphEdgeIndexInVertex;

				utils::containers::swapLastAndPop(vertex->m_graphEdges, foundGraphEdge->graphEdgeIndexInVertex);
			}
			else
			{
				vertex->m_graphEdges.pop_back();
			}

			if (foundGraphEdge->graphEdgeIndexInFace != (face->m_graphEdges.size() - 1))
			{
				face->m_graphEdges.back()->graphEdgeIndexInFace = foundGraphEdge->graphEdgeIndexInFace;

				utils::containers::swapLastAndPop(face->m_graphEdges, foundGraphEdge->graphEdgeIndexInFace);
			}
			else
			{
				face->m_graphEdges.pop_back();
			}

            //Check if Vertex needs to be deleted
			if (vertex->m_graphEdges.empty())
			{
				if (vertex->m_vertexIndexInVector != (halfEdgeMesh->m_vertices.size() - 1))
				{
					halfEdgeMesh->m_vertices.back()->m_vertexIndexInVector = vertex->m_vertexIndexInVector;
					utils::containers::swapLastAndPop(halfEdgeMesh->m_vertices, vertex->m_vertexIndexInVector);
				}
				else
				{
					halfEdgeMesh->m_vertices.pop_back();
				}

				vertex->m_halfEdge->m_vertex = nullptr;
			}
			else
			{
				ExtendedHalfEdge* neighborFaceHalfEdge = vertex->m_graphEdges.front()->face->m_halfEdge;

				while (neighborFaceHalfEdge->m_vertex != vertex)
				{
					ExtendedHalfEdge* neighborFaceNextHalfEdge = neighborFaceHalfEdge->m_next;
					neighborFaceHalfEdge = neighborFaceNextHalfEdge;
				}

				vertex->m_halfEdge = neighborFaceHalfEdge;
			}


			//---EDGE_DELETION---
			if (halfEdge->m_twin == nullptr)
			{
				deleteEdgeVaoData(mesh, halfEdge->m_edge);

				if (halfEdge->m_edge->m_edgeIndexInVector != (halfEdgeMesh->m_edges.size() - 1))
				{
					halfEdgeMesh->m_edges.back()->m_edgeIndexInVector = halfEdge->m_edge->m_edgeIndexInVector;
					utils::containers::swapLastAndPop(halfEdgeMesh->m_edges, halfEdge->m_edge->m_edgeIndexInVector);
				}
				else
				{
					halfEdgeMesh->m_edges.pop_back();
				}

			}
			else
			{
				if (halfEdge->m_edge->m_halfEdge == halfEdge)
				{
					halfEdge->m_edge->m_halfEdge = halfEdge->m_twin;
				}

				halfEdge->m_twin->m_twin = nullptr;
			}

			//---HALF_EDGE_DELETION---
			if (halfEdge->m_halfEdgeIndexInVector != (halfEdgeMesh->m_halfEdges.size() - 1))
			{
				halfEdgeMesh->m_halfEdges.back()->m_halfEdgeIndexInVector = halfEdge->m_halfEdgeIndexInVector;
				utils::containers::swapLastAndPop(halfEdgeMesh->m_halfEdges, halfEdge->m_halfEdgeIndexInVector);
			}
			else
			{
				halfEdgeMesh->m_halfEdges.pop_back();
			}


			//go next
			halfEdge = nextHalfEdge;
		} while (halfEdge != face->m_halfEdge);

		if (face->m_faceIndexInVector != (halfEdgeMesh->m_faces.size() - 1))
		{
			halfEdgeMesh->m_faces.back()->m_faceIndexInVector = face->m_faceIndexInVector;
			utils::containers::swapLastAndPop(halfEdgeMesh->m_faces, face->m_faceIndexInVector);
		}
		else
		{
			halfEdgeMesh->m_faces.pop_back();
		}

	}

	void deleteEdgeVaoData(Mesh* mesh, ExtendedEdge* edge)
	{
        LineBufferStorage* lineBufferStorage = Renderer::s_bufferRegistry.queryBuffer<LineBufferStorage>();

        LineBufferStorage::MeshBuffMap& meshLinesMap = lineBufferStorage->meshBuffMap;

        BufferData<RendererBuffersData::LineVertex>* lineBufferData;
        lineBufferStorage->getBufferData(mesh, lineBufferData);
        std::vector<RendererBuffersData::LineVertex>& lineBufferVertices = lineBufferData->vertices;


		//---VAO_DATA_SWAP---
		if (edge->m_EdgeLineIndex != (lineBufferVertices.size() - 4))
		{
			utils::containers::reverseSubrange(lineBufferVertices, lineBufferVertices.size() - 4, lineBufferVertices.size() - 1);
			for (int i = edge->m_EdgeLineIndex; i <= edge->m_EdgeLineIndex + 3; ++i)
			{
				utils::containers::swapLastAndPop(lineBufferVertices, i);
			}

			mesh->m_halfEdgeStructure->m_edges.back()->m_EdgeLineIndex = edge->m_EdgeLineIndex;

		}
		else
		{
			for (int i = lineBufferVertices.size() - 1; i >= edge->m_EdgeLineIndex; --i)
			{
				lineBufferVertices.erase(lineBufferVertices.begin() + i);
			}

			if (lineBufferVertices.empty())
			{
				meshLinesMap.erase(mesh);
			}

		}

	}

	void deleteFaceVaoData(Mesh* mesh, ExtendedFace* face)
	{
        MeshBufferStorage* meshBufferStorage = Renderer::s_bufferRegistry.queryBuffer<MeshBufferStorage>();

        MeshBufferStorage::MeshMatsMap& meshMatsMap = meshBufferStorage->meshMatsMap;
        MeshBufferStorage::MatBuffMap& materialVertsMap = meshMatsMap.find(mesh)->second;

        BufferData<RendererBuffersData::MeshVertex>* meshBufferData;
        meshBufferStorage->getBufferData(mesh, face->material, meshBufferData);
        std::vector<RendererBuffersData::MeshVertex>& meshBufferVertices = meshBufferData->vertices;

		std::map<Material*, std::vector<FaceTriangle>>& materialTrianglesMap = mesh->m_halfEdgeStructure->m_faceTriangles;
		auto materialTrianglesIt = materialTrianglesMap.find(face->material);
		std::vector<FaceTriangle>& faceTriangles = materialTrianglesIt->second;

		while (!face->faceTriangleIndices.empty())
		{
			FaceTriangleIndex delFaceTriangleIndex = face->faceTriangleIndices.front();
			FaceTriangle& delFaceTriangle = faceTriangles.at(delFaceTriangleIndex);

			if (delFaceTriangleIndex != faceTriangles.size() - 1)
			{
				faceTriangles.back().indexInVAO = delFaceTriangle.indexInVAO;

				//---VAO_DATA_SWAP---
				utils::containers::reverseSubrange(meshBufferVertices, meshBufferVertices.size() - 3, meshBufferVertices.size() - 1);
				for (int i = delFaceTriangle.indexInVAO; i <= delFaceTriangle.indexInVAO + 2; ++i)
				{
					utils::containers::swapLastAndPop(meshBufferVertices, i);
				}
			}
			else
			{
				for (int i = meshBufferVertices.size() - 1; i >= delFaceTriangle.indexInVAO; --i)
				{
					meshBufferVertices.erase(meshBufferVertices.begin() + i);
				}

				if (meshBufferVertices.empty())
				{
					materialVertsMap.erase(face->material);
					if (materialVertsMap.empty())
					{
						meshMatsMap.erase(mesh);
					}
				}

			}

			ExtendedFace* lastTriangleFace = faceTriangles.back().face;
			lastTriangleFace->faceTriangleIndices.at(faceTriangles.back().indexInFace) = delFaceTriangleIndex;

			//---M_FACE_TRIANGLES_SWAP---
			utils::containers::swapLastAndPop(faceTriangles, delFaceTriangleIndex);

			if (faceTriangles.empty())
			{
				materialTrianglesMap.erase(materialTrianglesIt);
			}

			if (face->faceTriangleIndices.size() != 1)
			{
				faceTriangles.at(face->faceTriangleIndices.back()).indexInFace = 0;
			}

			//---FACE_TRIANGLE_INDICES_SWAP---
			utils::containers::swapLastAndPop(face->faceTriangleIndices, 0);
		}
	}
};
