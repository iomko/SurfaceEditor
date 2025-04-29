#pragma once
#include <iostream>

#include "Callback.h"
#include "../ViewPortsHolder.h"
#include "../AABBBoundingRegion.h"
#include "../Core/Input.h"
#include "../MeshSelectionManager.h"

class DeselectMeshCallBack : public Callback<>, public Observer
{
public:
	void execute() override
	{
		RetClosestOctreeDataCallable closestOctreeDataCallable;

		OctreeNodeDataParams output;
		closestOctreeDataCallable.invoke(output);

		Mesh* mesh = output.meshFacePair.first;

		MeshSelectionManager::unregisterMesh(*ViewPortsHolderContext::m_objectSelectionHolder, mesh);

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
