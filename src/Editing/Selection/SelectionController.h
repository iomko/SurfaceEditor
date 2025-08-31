#pragma once
#include "MeshSelectionManager.h"
#include "FaceSelectionManager.h"

class SelectionController
{
public:
	SelectionController() : m_faceManager(m_holder), m_meshManager(m_holder) {}

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

private:
	SelectionHolder m_holder;
	FaceSelectionManager m_faceManager;
	MeshSelectionManager m_meshManager;
};