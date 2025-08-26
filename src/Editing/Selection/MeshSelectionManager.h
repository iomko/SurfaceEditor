#pragma once
#include "Scene/Mesh.h"
#include "SelectionHolder.h"

class MeshSelectionManager
{
public:
	MeshSelectionManager(SelectionHolder& holder) : m_holder(holder) {}

	void registerMesh(Mesh* mesh)
	{
		if (mesh != nullptr && !mesh->m_selected)
		{
			m_holder.meshes.emplace_back(mesh);
			mesh->m_selected = true;
			mesh->m_selectionIndex = m_holder.meshes.size() - 1;
		}
	}

	void unregisterMesh(Mesh* mesh)
	{
		if (mesh != nullptr && mesh->m_selected)
		{
			int indexInSelection = mesh->m_selectionIndex;
			std::vector<Mesh*>& selectionVector = m_holder.meshes;
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
			auto faceSelectionsIt = m_holder.faces.find(mesh);
			if(faceSelectionsIt != m_holder.faces.end())
			{
				//vymaz vsetky selected faces selectnuteho meshu
				std::vector<HalfEdgeDS::Face*>& faceSelections = m_holder.faces.find(mesh)->second;

				while (!faceSelections.empty()) {
					HalfEdgeDS::Face* face = faceSelections.back();

					face->m_selected = false;
					face->m_selectionIndex = -1;

					faceSelections.pop_back();
				}

				m_holder.faces.erase(mesh);
			}
		}
	}

private:
	SelectionHolder& m_holder;
};