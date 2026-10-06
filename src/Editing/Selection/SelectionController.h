#pragma once
#include "MeshSelectionManager.h"
#include "FaceSelectionManager.h"
#include "../../UI/SelectionRectangle.h"

enum class SelectionMode
{
	MESH,
	FACE,
	EDGE,
	VERTEX
};

class SelectionController
{
public:
	SelectionController();

	const SelectionHolder& getHolder() const
	{
		return m_holder;
	}

	void registerFace(Mesh* mesh, ExtendedFace* face)
	{
		m_faceManager.registerFace(face, mesh);
	}

	void unregisterFace(Mesh* mesh, ExtendedFace* face)
	{
		m_faceManager.unregisterFace(face, mesh);
	}

	void registerMesh(Mesh* mesh)
	{
		m_meshManager.registerMesh(mesh);
	}

	void unregisterMesh(Mesh* mesh)
	{
		m_meshManager.unregisterMesh(mesh);
	}

	void clear();

	const SelectionMode& selectionMode() const
	{
		return m_selectionMode;
	}

	void setSelectionMode(const SelectionMode& selectionMode)
	{
		m_selectionMode = selectionMode;
	}

	const bool& newSelectionEvent() const
	{
		return m_newSelectionEvent;
	}

	void setNewSelectionEvent(bool isNewSelectionEvent)
	{
		m_newSelectionEvent = isNewSelectionEvent;
	}

	void setSelectionModeActive(const bool& isActive)
	{
		m_selectionModeActive = isActive;
	}

	void createSelectionRectangle()
	{
		m_selectionRectangle.create();
	}

	void drawSelectionRectangle();

	void updateSelectionRectangle(int width, int height, const RectanglePos& rectanglePos);

	void clearSelectionRectangle()
	{
		m_selectionRectangle.clear();
	}

private:
	SelectionHolder 	 m_holder;
	FaceSelectionManager m_faceManager;
	MeshSelectionManager m_meshManager;
	SelectionRectangle   m_selectionRectangle;
	SelectionMode		 m_selectionMode = SelectionMode::MESH;
	bool				 m_newSelectionEvent;
	bool 				 m_selectionModeActive;
};