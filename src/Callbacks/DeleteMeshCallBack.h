#pragma once
#include "../Commands/CommandRegistry.h"
#include "../Commands/DeleteFaceCommand.h"
#include "../Structures/ExtendedHalfEdge.h"


class DeleteMeshCallBack : public Callback<MeshParams>, public Observer
{
public:
    DeleteMeshCallBack(CommandRegistry* commandRegistry) : m_commandRegistry(commandRegistry) {}

	void execute(const MeshParams& iParams) override
	{
        Mesh* selectedMesh = iParams.m_mesh;
        std::vector<ExtendedFace*> facesToDelete = selectedMesh->getHalfEdgeStructure()->m_faces;

        for(ExtendedFace* face : facesToDelete) {
        
            SingleFaceParams singleFaceParams;
            singleFaceParams.mesh = selectedMesh;
            singleFaceParams.face = face;

            DeleteFaceCommand* deleteFaceCommand = m_commandRegistry->getCommand<DeleteFaceCommand>();
            deleteFaceCommand->execute(singleFaceParams); 
        
        }
	}

private:
    CommandRegistry* m_commandRegistry = nullptr;
};
