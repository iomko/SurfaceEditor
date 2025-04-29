#pragma once
#include "Mesh.h"
#include "ObjectSelectionHolder.h"

class MeshSelectionManager
{
public:
	static void registerMesh(ObjectSelectionHolder& holder, Mesh* mesh)
	{
		if (mesh != nullptr && !mesh->m_selected)
		{
			holder.m_meshes.emplace_back(mesh);
			mesh->m_selected = true;
			mesh->m_selectionIndex = holder.m_meshes.size() - 1;
		}
	}
	static void unregisterMesh(ObjectSelectionHolder& holder, Mesh* mesh)
	{
		if (mesh != nullptr && mesh->m_selected)
		{
			int indexInSelection = mesh->m_selectionIndex;
			std::vector<Mesh*>& selectionVector = holder.m_meshes;
			if (indexInSelection != selectionVector.size() - 1)
			{
				//vymenime entry s poslednym vo vectore

				Mesh* lastMesh = selectionVector.back();
				selectionVector.at(indexInSelection) = lastMesh;

				lastMesh->m_selectionIndex = indexInSelection;

				selectionVector.back() = mesh;
			}
			mesh->m_selected = false;
			mesh->m_selectionIndex = -1;
			selectionVector.erase(selectionVector.end() - 1);

			//selected faces nemusia existovat
			auto faceSelectionsIt = holder.m_faces.find(mesh);
			if(faceSelectionsIt != holder.m_faces.end())
			{
				//vymaz vsetky selected faces selectnuteho meshu
				std::vector<HalfEdgeDS::Face*>& faceSelections = holder.m_faces.find(mesh)->second;

				while (!faceSelections.empty()) {
					HalfEdgeDS::Face* face = faceSelections.back();

					face->m_selected = false;
					face->m_selectionIndex = -1;

					faceSelections.pop_back();
				}

				holder.m_faces.erase(mesh);
			}
		}
	}
};