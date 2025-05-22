#pragma once
#include "../FaceSelectionManager.h"

class DeselectFaceCallBack : public Callback<>, public Observer
{
public:
	virtual void execute() override
	{
		Camera* camera = ViewPortsHolderContext::m_camera;
		Window* window = ViewPortsHolderContext::m_window;
		Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;
		std::pair<SceneRes::MeshFacePair, glm::vec3> meshFaceHitPair = SceneUtilities::retClosestHitData(camera, window, scene->m_res);

		SceneRes::MeshFacePair meshFacePair = meshFaceHitPair.first;

		Mesh* mesh = meshFacePair.first;
		HalfEdgeDS::Face* face = meshFacePair.second;

		FaceSelectionManager::unregisterFace(*ViewPortsHolderContext::m_objectSelectionHolder, face, mesh);

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