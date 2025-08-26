#include "FaceSelectionManager.h"
#include "../../ViewPortsController.h"
#include "../../Utils/ContainerUtils.h"

FaceSelectionManager::FaceSelectionManager(SelectionHolder& holder)
	: m_holder(holder)
{}

void FaceSelectionManager::registerFace(HalfEdgeDS::Face* face, Mesh* mesh)
{
	if ((mesh != nullptr && mesh->m_selected) &&
		(face != nullptr && !face->m_selected))
	{
		std::vector<HalfEdgeDS::Face*>& selectionVector = m_holder.faces[mesh];
		selectionVector.emplace_back(face);

		RendererStageData::MeshMatsMap& meshMatsMap = Renderer::s_stageData.meshMatsMap;
		std::vector<HalfEdgeDS::FaceTriangle>& faceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second;

		RendererStageData::MatVertsMap& materialVertsMap = meshMatsMap.find(mesh)->second;
		std::vector<RendererStageData::MeshVertex>& meshVaoVertices = materialVertsMap.find(face->material)->second;

		for (FaceTriangleIndex faceTriangleIndex : face->faceTriangleIndices)
		{
			int faceVaoIndex = faceTriangles.at(faceTriangleIndex).indexInVAO;
			meshVaoVertices.at(faceVaoIndex).isHighlited = true;
			meshVaoVertices.at(faceVaoIndex + 1).isHighlited = true;
			meshVaoVertices.at(faceVaoIndex + 2).isHighlited = true;
		}

		face->m_selected = true;
		face->m_selectionIndex = selectionVector.size() - 1;
	}
}

void FaceSelectionManager::unregisterFace(HalfEdgeDS::Face* face, Mesh* mesh)
{
	if ((mesh != nullptr && mesh->m_selected) &&
		(face != nullptr && face->m_selected))
	{
		//musime si ziskat vector z Mesh*
		int indexInSelection = face->m_selectionIndex;
		std::vector<HalfEdgeDS::Face*>& selectionVector = m_holder.faces.find(mesh)->second;

		RendererStageData::MeshMatsMap& meshMatsMap = Renderer::s_stageData.meshMatsMap;
		std::vector<HalfEdgeDS::FaceTriangle>& faceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second;

		RendererStageData::MatVertsMap& materialVertsMap = meshMatsMap.find(mesh)->second;
		std::vector<RendererStageData::MeshVertex>& meshVaoVertices = materialVertsMap.find(face->material)->second;

		if (indexInSelection != selectionVector.size() - 1)
		{
			//vymenime entry s poslednym vo vectore
			selectionVector.back()->m_selectionIndex = indexInSelection;
			utils::containers::swapWithLast(selectionVector, indexInSelection);
		}

		for (FaceTriangleIndex faceTriangleIndex : face->faceTriangleIndices)
		{
			int faceVaoIndex = faceTriangles.at(faceTriangleIndex).indexInVAO;
			meshVaoVertices.at(faceVaoIndex).isHighlited = false;
			meshVaoVertices.at(faceVaoIndex + 1).isHighlited = false;
			meshVaoVertices.at(faceVaoIndex + 2).isHighlited = false;
		}

		face->m_selected = false;
		face->m_selectionIndex = -1;

		selectionVector.erase(selectionVector.end() - 1);


		if (selectionVector.empty()) {
			m_holder.faces.erase(mesh);
		}
	}
}
