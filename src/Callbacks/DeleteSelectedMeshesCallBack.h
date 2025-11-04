#pragma once
#include "../Commands/CommandRegistry.h"


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

            auto* deleteMeshCommand = CommandRegistry::instance().getCommand("DeleteMesh");
            if(deleteMeshCommand) deleteMeshCommand->execute(meshParams); 

			ViewPortsHolderContext::s_selectionController->unregisterMesh(mesh);
		}
	}

private:
};
