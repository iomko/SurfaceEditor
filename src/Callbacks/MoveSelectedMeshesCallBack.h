#pragma once
#include "../Params/OperationParams.h"
#include "../ViewPortsController.h"
#include "../Utils/InterpolationUtils.h"
#include "../Utils/GeometryUtils.h"
#include "../Renderer/MaterialRegistry.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/MoveMeshCommand.h"


class MoveSelectedMeshesCallBack : public Callback<MoveSelectedMeshesParams>, public Observer
{
public:
    MoveSelectedMeshesCallBack(CommandRegistry* commandRegistry) : m_commandRegistry(commandRegistry) {}

	void execute(const MoveSelectedMeshesParams& iParams) override
	{
        MoveMeshParams moveMeshParams;
        moveMeshParams.moveByVector = iParams.moveByVector;

        SelectionController* selectionController = ViewPortsHolderContext::s_selectionController;
		const SelectionHolder& selectionHolder = selectionController->getHolder();
		const std::vector<Mesh*>& selectedMeshes = selectionHolder.meshes;
        MoveMeshCommand* moveMeshCommand = m_commandRegistry->getCommand<MoveMeshCommand>();

    	for(Mesh* selectedMesh : selectedMeshes) {
            moveMeshParams.mesh = selectedMesh;
            moveMeshCommand->execute(moveMeshParams);
            selectedMesh->m_transform = glm::mat4(1.0f);
        }
    }

private:
    CommandRegistry* m_commandRegistry = nullptr;
};
