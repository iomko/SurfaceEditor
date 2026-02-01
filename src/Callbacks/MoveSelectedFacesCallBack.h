#pragma once
#include "../Params/OperationParams.h"
#include "../ViewPortsController.h"
#include "../Utils/InterpolationUtils.h"
#include "../Utils/GeometryUtils.h"
#include "../Renderer/MaterialRegistry.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/MoveVertexCommand.h"


class MoveSelectedFacesCallBack : public Callback<MoveSelectedFacesParams>, public Observer
{
public:
    MoveSelectedFacesCallBack(CommandRegistry* commandRegistry) : m_commandRegistry(commandRegistry) {}

	void execute(const MoveSelectedFacesParams& iParams) override
	{
        glm::vec3 moveByVector = iParams.moveByVector;

        //Get selected meshes
		SelectionController*  selectionController = ViewPortsHolderContext::s_selectionController;
		const SelectionHolder& selectionHolder = selectionController->getHolder();
		Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
		const std::vector<Mesh*>& selectedMeshes = selectionHolder.meshes;

		for(Mesh* selectedMesh : selectedMeshes)
		{
            const std::vector<ExtendedFace*>& selectedFaces = selectionHolder.faces.find(selectedMesh)->second;
            //musime ziskat vsetky unique vertices
            std::unordered_set<ExtendedVertex*> verticesToMove;

            for(ExtendedFace* face : selectedFaces) {
                face->m_transform[3] += glm::vec4(glm::vec3(iParams.moveByVector), 0.0f);

                for (auto it = face->faceVertexBegin(); it != face->faceVertexEnd(); ++it) {
                    
                    ExtendedVertex* vertex = &(*it);
                    verticesToMove.insert(vertex);
                }
            }

            VertexParams vertexParams;
            vertexParams.mesh = selectedMesh;
            vertexParams.newPosition = moveByVector;

            for(ExtendedVertex* vertex : verticesToMove) {
                
                vertexParams.vertex = vertex;

                MoveVertexCommand* moveVertexCommand = m_commandRegistry->getCommand<MoveVertexCommand>();
                moveVertexCommand->execute(vertexParams);
            }


            selectedMesh->calculateMeshBounds();
		}

    }

private:
    CommandRegistry* m_commandRegistry = nullptr;
};
