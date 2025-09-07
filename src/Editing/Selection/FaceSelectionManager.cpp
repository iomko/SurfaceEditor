#include "FaceSelectionManager.h"
#include "../../ViewPortsController.h"
#include "../../Utils/ContainerUtils.h"

FaceSelectionManager::FaceSelectionManager(SelectionHolder& holder)
	: m_holder(holder)
{}

void FaceSelectionManager::registerFace(ExtendedFace* face, Mesh* mesh)
{
	if ((mesh != nullptr && mesh->m_selected) &&
		(face != nullptr && !face->m_selected))
	{
		std::vector<ExtendedFace*>& selectionVector = m_holder.faces[mesh];
		selectionVector.emplace_back(face);

		std::vector<FaceTriangle>& faceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second;

        //potrebujeme ziskat materialVaoVertices
        MeshBufferStorage* meshBufferStorage = Renderer::s_bufferRegistry.queryBuffer<MeshBufferStorage>();
        BufferData<RendererBuffersData::MeshVertex>* meshBufferData;
        meshBufferStorage->getBufferData(mesh, face->material, meshBufferData);
        std::vector<RendererBuffersData::MeshVertex>& meshVaoVertices = meshBufferData->vertices;

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

void FaceSelectionManager::unregisterFace(ExtendedFace* face, Mesh* mesh)
{
	if ((mesh != nullptr && mesh->m_selected) &&
		(face != nullptr && face->m_selected))
	{
		//musime si ziskat vector z Mesh*
		int indexInSelection = face->m_selectionIndex;
		std::vector<ExtendedFace*>& selectionVector = m_holder.faces.find(mesh)->second;

		std::vector<FaceTriangle>& faceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second;

        //potrebujeme ziskat materialVaoVertices
        MeshBufferStorage* meshBufferStorage = Renderer::s_bufferRegistry.queryBuffer<MeshBufferStorage>();
        BufferData<RendererBuffersData::MeshVertex>* meshBufferData;
        meshBufferStorage->getBufferData(mesh, face->material, meshBufferData);
        std::vector<RendererBuffersData::MeshVertex>& meshVaoVertices = meshBufferData->vertices;

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
