#include "SelectionController.h"

SelectionController::SelectionController()
	: m_faceManager(m_holder), m_meshManager(m_holder), m_selectionModeActive{}
{ }

void SelectionController::clear()
{
    if (!m_holder.faces.empty())
	{
		for (auto& [mesh, faces] : m_holder.faces)
		{
			mesh->m_selected = false;
			mesh->m_selectionIndex = -1;

			m_meshManager.unhighlight(mesh);

			for (auto& face : faces)
			{
				face->m_selected = false;
				face->m_selectionIndex = -1;

				//TODO unhighlight faces also
			}
		}
	}

	if (!m_holder.meshes.empty())
	{
		for (auto& mesh : m_holder.meshes)
		{
			mesh->m_selected = false;
			mesh->m_selectionIndex = -1;

			m_meshManager.unhighlight(mesh);
		}
	}

	m_holder.faces.clear();
	m_holder.meshes.clear();
}

void SelectionController::drawSelectionRectangle()
{
	if (m_selectionModeActive)
	{
		m_selectionRectangle.draw();
	}
}

void SelectionController::updateSelectionRectangle(int width, int height, const RectanglePos& rectanglePos)
{
	m_selectionRectangle.update(width, height, rectanglePos);
}