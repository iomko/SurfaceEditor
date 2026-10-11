#pragma once
#include "Scene/Mesh.h"
#include "SelectionHolder.h"

class MeshSelectionManager
{
public:
	MeshSelectionManager(SelectionHolder& holder) : m_holder(holder) {}

	void registerMesh(Mesh* mesh);

	void unregisterMesh(Mesh* mesh);

	void highlight(Mesh* mesh);

	void unhighlight(Mesh* mesh);

private:
	SelectionHolder& m_holder;
};