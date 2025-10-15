#pragma once
#include "../Commands/DeleteFaceCommand.h"
#include "../Params/OperationParams.h"
#include "../Commands/CommandRegistry.h"
#include "../Utils/ContainerUtils.h"
#include "Structures/ExtendedHalfEdge.h"

class DeleteSelectedFacesCallBack : public Callback<>, public Observer
{
public:
    DeleteSelectedFacesCallBack(CommandRegistry* commandRegistry) : m_commandRegistry(commandRegistry) {} 

	void execute() override
	{
        //Get selected meshes
		SelectionController*  selectionController = ViewPortsHolderContext::s_selectionController;
		const SelectionHolder& selectionHolder = selectionController->getHolder();
		Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
		const std::vector<Mesh*>& selectedMeshes = selectionHolder.meshes;

		for(Mesh* selectedMesh : selectedMeshes)
		{
			const std::vector<ExtendedFace*>& selectedFaces = selectionHolder.faces.find(selectedMesh)->second;

            //Go through each selected Face
			while(!selectedFaces.empty())
			{
				ExtendedFace* selectedFace = selectedFaces.back();

                SingleFaceParams singleFaceParams;
                singleFaceParams.mesh = selectedMesh;
                singleFaceParams.face = selectedFace;

                DeleteFaceCommand* deleteFaceCommand = m_commandRegistry->getCommand<DeleteFaceCommand>(); 
                deleteFaceCommand->execute(singleFaceParams);

		        //Delete face/mesh from selection	
				if(selectedMesh->m_halfEdgeStructure->m_faces.empty())
				{
					selectionController->unregisterMesh(selectedMesh);
				} else
				{
					selectionController->unregisterFace(selectedMesh, selectedFace);
				}
			}

            selectedMesh->calculateMeshBounds();
		}
	}

private:

    CommandRegistry* m_commandRegistry = nullptr;
};
