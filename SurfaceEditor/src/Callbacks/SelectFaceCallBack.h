#pragma once
#include "Callback.h"
#include "../Callables/RetClosestOctreeDataCallable.h"
#include "../FaceSelectionManager.h"

class SelectFaceCallBack : public Callback<>, public Observer
{
public:
	virtual void execute() override
	{
		RetClosestOctreeDataCallable closestOctreeDataCallable;

		OctreeNodeDataParams output;
		closestOctreeDataCallable.invoke(output);

		Mesh* mesh = output.meshFacePair.first;
		HalfEdgeDS::Face* face = output.meshFacePair.second;

		FaceSelectionManager::registerFace(*ViewPortsHolderContext::m_objectSelectionHolder, face, mesh);

		//LOG
		std::cout << "-----MESH_SELECTIONS-----" << std::endl;
		for (Mesh* selectedMesh : ViewPortsHolderContext::m_objectSelectionHolder->m_meshes)
		{
			std::cout << "---Mesh: " << selectedMesh->m_meshID << std::endl;
		}

		//LOG
		std::cout << "-----FACE_SELECTIONS-----" << std::endl;
		for (auto& [selectedMesh, faceVector] : ViewPortsHolderContext::m_objectSelectionHolder->m_faces)
		{
			std::cout << "---Mesh: " << selectedMesh->m_meshID << std::endl;
			for (HalfEdgeDS::Face* selectedFace : faceVector)
			{
				std::cout << "------Face: " << selectedFace->m_selectionIndex << std::endl;
			}
		}
	}
};