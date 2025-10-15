#pragma once
#include "../Commands/CommandRegistry.h"
#include "../Commands/DeleteMeshCommand.h"


class DeleteSelectedMeshesCallBack : public Callback<>, public Observer
{
public:
    DeleteSelectedMeshesCallBack(CommandRegistry* commandRegistry) : m_commandRegistry(commandRegistry) {}

	void execute() override
	{
		const SelectionHolder& selectionsHolder = ViewPortsHolderContext::s_selectionController->getHolder();
		const std::vector<Mesh*>& selectedMeshes = selectionsHolder.meshes;

		while(!selectedMeshes.empty())
		{
			Mesh* mesh = selectedMeshes.back();

            MeshParams meshParams;
            meshParams.m_mesh = mesh;

            DeleteMeshCommand* deleteMeshCommand = m_commandRegistry->getCommand<DeleteMeshCommand>();
            deleteMeshCommand->execute(meshParams); 

			ViewPortsHolderContext::s_selectionController->unregisterMesh(mesh);
		}
	}

private:
    
    CommandRegistry* m_commandRegistry = nullptr;
};
