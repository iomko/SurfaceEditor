#pragma once
#include "SelectionHolder.h"

class FaceSelectionManager
{
public:
	FaceSelectionManager(SelectionHolder& holder);

	void registerFace(HalfEdgeDS::Face* face, Mesh* mesh);
	void unregisterFace(HalfEdgeDS::Face* face, Mesh* mesh);

private:
	SelectionHolder& m_holder;
};