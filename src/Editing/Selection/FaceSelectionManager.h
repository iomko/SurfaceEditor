#pragma once
#include "SelectionHolder.h"

class FaceSelectionManager
{
public:
	FaceSelectionManager(SelectionHolder& holder);

	void registerFace(ExtendedFace* face, Mesh* mesh);

	void unregisterFace(ExtendedFace* face, Mesh* mesh);

	void highlight(ExtendedFace* face);

	void unhighlight(ExtendedFace* face);

private:
	SelectionHolder& m_holder;
};