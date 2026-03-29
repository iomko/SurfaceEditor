#include "SelectionController.h"

void SelectionController::clear()
{
    if (!m_holder.faces.empty())
		{
			for (auto& [mesh, faces] : m_holder.faces)
			{
				mesh->m_selected = false;
				mesh->m_selectionIndex = -1;

				for (auto& face : faces)
				{
					face->m_selected = false;
					face->m_selectionIndex = -1;
				}
			}
		}

		if (!m_holder.meshes.empty())
		{
			for (auto& mesh : m_holder.meshes)
			{
				mesh->m_selected = false;
				mesh->m_selectionIndex = -1;
			}
		}
		
		m_holder.faces.clear();
		m_holder.meshes.clear();
}
