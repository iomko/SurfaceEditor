#pragma once
#include "../Utils/ContainerUtils.h"
#include "DataStructures/ExtendedHalfEdge.h"

class DeleteSelectedFacesCallBack : public Callback<>, public Observer
{
public:
	void execute() override
	{
		SelectionController* selectionController = ViewPortsHolderContext::s_selectionController;
		const SelectionHolder& selectionHolder = selectionController->getHolder();
		Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
		const std::vector<Mesh*>& selectedMeshes = selectionHolder.meshes;

		for(Mesh* selectedMesh : selectedMeshes)
		{
			const std::vector<ExtendedFace*>& selectedFaces = selectionHolder.faces.find(selectedMesh)->second;

			while(!selectedFaces.empty())
			{
				ExtendedFace* selectedFace = selectedFaces.back();

				//---DELETE_FROM_OCTREES---
				scene->deleteFaceFromOctrees(selectedMesh, selectedFace);
				//---DELETE_FROM_OCTREES---

				//---DELETE_FROM_HALFEDGE_STRUCTURE---
				deleteFaceVaoData(selectedMesh, selectedFace);
				deleteFaceMeshData(selectedMesh, selectedFace);
				//---DELETE_FROM_HALFEDGE_STRUCTURE---

				//---DELETE_FROM_SELECTIONS---
				if(selectedMesh->m_halfEdgeStructure->m_faces.empty())
				{
					selectionController->unregisterMesh(selectedMesh);
				} else
				{
					selectionController->unregisterFace(selectedMesh, selectedFace);
				}
				//---DELETE_FROM_SELECTIONS---
			}
		}
	}

private:

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

			//teraz potrebujem vymazat dany foundGraphEdge
			//na to aby som to mohol spravit tak
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

			//dobre teraz sme vymazali graphData pri danej vertex

			//teraz je potrebne sa pozriet na to ci mame vymazat aj samotny vertex z HalfEdge
			//alebo nie

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


				//dobre tuto sa vymazala vertex, tym ze sa vymazala tak sme si isty, ze tento vertex
				//uz nepatri ziadnej inej face

				//preto musime vymazat vertex aj z halfEdge
				vertex->m_halfEdge->m_vertex = nullptr;
			}
			else
			{
				//musime najst novy halfEdge ktory bude pripadat vertexu
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
				//neexistuje twin

				deleteEdgeVaoData(mesh, halfEdge->m_edge);

				//tak vymazeme tento edge
				//na to aby sme vymazali rychlo edge tak musime vediet o indexe na ktorom sa nachadza
				//vo vectore


				if (halfEdge->m_edge->m_edgeIndexInVector != (halfEdgeMesh->m_edges.size() - 1))
				{
					//tak vieme ze sa nenachadzal na konci
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
				//tak vieme ze sa nenachadzal na konci
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

		//nakonci uz len vymazem face

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
		RendererStageData::MeshLinesMap& meshLinesMap = Renderer::s_stageData.meshLinesMap;
		auto meshLinesVaoMapIt = meshLinesMap.find(mesh);

		std::vector<RendererStageData::LineVertex>& meshLinesVaoVector = meshLinesVaoMapIt->second;


		//---VAO_DATA_SWAP---
		//before
		if (edge->m_EdgeLineIndex != (meshLinesVaoVector.size() - 4))
		{
			utils::containers::reverseSubrange(meshLinesVaoVector, meshLinesVaoVector.size() - 4, meshLinesVaoVector.size() - 1);
			for (int i = edge->m_EdgeLineIndex; i <= edge->m_EdgeLineIndex + 3; ++i)
			{
				utils::containers::swapLastAndPop(meshLinesVaoVector, i);
			}

			mesh->m_halfEdgeStructure->m_edges.back()->m_EdgeLineIndex = edge->m_EdgeLineIndex;

		} else
		{
			for (int i = meshLinesVaoVector.size() - 1; i >= edge->m_EdgeLineIndex; --i)
			{
				meshLinesVaoVector.erase(meshLinesVaoVector.begin() + i);
			}

			if(meshLinesVaoVector.empty())
			{
				meshLinesMap.erase(meshLinesVaoMapIt);
			}

		}

	}

	//tymto vymazeme vao data z meshu
	void deleteFaceVaoData(Mesh* mesh, ExtendedFace* face)
	{
		RendererStageData::MeshMatsMap& meshMatsMap = Renderer::s_stageData.meshMatsMap;
		auto meshVaoMapIt = meshMatsMap.find(mesh);

		RendererStageData::MatVertsMap& materialVertsMap = meshVaoMapIt->second;
		auto materialMapIt = materialVertsMap.find(face->material);
		std::vector<RendererStageData::MeshVertex>& facesVao = materialMapIt->second;

		std::map<Material*, std::vector<FaceTriangle>>& materialTrianglesMap = mesh->m_halfEdgeStructure->m_faceTriangles;
		auto materialTrianglesIt = materialTrianglesMap.find(face->material);
		std::vector<FaceTriangle>& faceTriangles = materialTrianglesIt->second;

		while (!face->faceTriangleIndices.empty())
		{
			FaceTriangleIndex delFaceTriangleIndex = face->faceTriangleIndices.front();
			FaceTriangle& delFaceTriangle = faceTriangles.at(delFaceTriangleIndex);

			//before
			if (delFaceTriangleIndex != faceTriangles.size() - 1)
			{
				faceTriangles.back().indexInVAO = delFaceTriangle.indexInVAO;

				//---VAO_DATA_SWAP---
				utils::containers::reverseSubrange(facesVao, facesVao.size() - 3, facesVao.size() - 1);
				for (int i = delFaceTriangle.indexInVAO; i <= delFaceTriangle.indexInVAO + 2; ++i)
				{
					utils::containers::swapLastAndPop(facesVao, i);
				}
			}
			else
			{
				for (int i = facesVao.size() - 1; i >= delFaceTriangle.indexInVAO; --i)
				{
					facesVao.erase(facesVao.begin() + i);
				}

				if (facesVao.empty())
				{
					materialVertsMap.erase(materialMapIt);
					if (materialVertsMap.empty())
					{
						meshMatsMap.erase(meshVaoMapIt);
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

			//before

			if (face->faceTriangleIndices.size() != 1)
			{
				faceTriangles.at(face->faceTriangleIndices.back()).indexInFace = 0;
			}

			//---FACE_TRIANGLE_INDICES_SWAP---
			utils::containers::swapLastAndPop(face->faceTriangleIndices, 0);
		}
	}
};
