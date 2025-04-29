#pragma once
#include "ObjectSelectionHolder.h"

class FaceSelectionManager
{
public:
	static void registerFace(ObjectSelectionHolder& holder, HalfEdgeDS::Face* face, Mesh* mesh)
	{
		if((mesh != nullptr && mesh->m_selected) &&
			(face != nullptr && !face->m_selected))
		{
			std::vector<HalfEdgeDS::Face*>& selectionVector = holder.m_faces[mesh];

			selectionVector.emplace_back(face);

			SceneRendererData::MeshFacesVaoMap& meshFacesVaoMap =
				ViewPortsHolderContext::m_viewPortsHolder->m_scene->m_rendererData.meshData.meshFacesVaoMap;

			std::vector<HalfEdgeDS::FaceTriangle>& faceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second;
			SceneRendererData::MaterialVaoMap& materialVaoMap = meshFacesVaoMap.find(mesh)->second;
			std::vector<MeshVertex>& meshVaoVertices = materialVaoMap.find(face->material)->second;

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

	static void unregisterFace(ObjectSelectionHolder& holder, HalfEdgeDS::Face* face, Mesh* mesh)
	{
		if((mesh != nullptr && mesh->m_selected) &&
			(face != nullptr && face->m_selected))
		{
			//musime si ziskat vector z Mesh*
			int indexInSelection = face->m_selectionIndex;
			std::vector<HalfEdgeDS::Face*>& selectionVector = holder.m_faces.find(mesh)->second;
			
			if(indexInSelection != selectionVector.size() - 1)
			{
				//vymenime entry s poslednym vo vectore
				HalfEdgeDS::Face* lastFace = selectionVector.back();
				selectionVector.at(indexInSelection) = lastFace;

				lastFace->m_selectionIndex = indexInSelection;

				selectionVector.back() = face;
			}
			face->m_selected = false;
			face->m_selectionIndex = -1;

			selectionVector.erase(selectionVector.end() - 1);


			if (selectionVector.empty()) {
				holder.m_faces.erase(mesh);
			}
		}
	}
};