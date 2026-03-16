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

	void clear()
	{
		for (auto& [mesh, faces] : m_holder.faces)
		{
			mesh->m_selected = false;
			
			for (auto& face : faces)
			{
				face->m_selected = false;
			}
		}

		for (auto& mesh : m_holder.meshes)
		{
			mesh->m_selected =false;
		}

		//TOTO TREBA LEPSIE NAPISAT

		m_holder.faces.clear();
		m_holder.meshes.clear();
	}

private:
	SelectionHolder m_holder;
	FaceSelectionManager m_faceManager;
	MeshSelectionManager m_meshManager;
};