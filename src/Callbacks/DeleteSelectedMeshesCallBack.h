#pragma once
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"


class DeleteSelectedMeshesCallBack : public Callback<>, public Observer
{
public:
    DeleteSelectedMeshesCallBack() {}

	void execute() override
	{
		const SelectionHolder& selectionsHolder = ViewPortsHolderContext::s_selectionController->getHolder();
		const std::vector<Mesh*>& selectedMeshes = selectionsHolder.meshes;

		while(!selectedMeshes.empty())
		{
			Mesh* mesh = selectedMeshes.back();

            MeshParams meshParams;
            meshParams.m_mesh = mesh;

            auto* deleteMeshCommand = CommandRegistry::instance().getCommand(DELETE_MESH_COMMAND);
            if(deleteMeshCommand) deleteMeshCommand->execute(meshParams); 

			ViewPortsHolderContext::s_selectionController->unregisterMesh(mesh);
		}
	}

private:
};
