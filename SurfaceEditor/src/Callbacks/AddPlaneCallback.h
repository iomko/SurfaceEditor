#pragma once
#include "Callback.h"
#include "../Commands/AddPlaneCommand.h"
#include "../Commands/CommandRegistry.h"

#include "../ViewPortsHolder.h"
#include "../Callbacks/InitMeshVaoDataCallBack.h"
#include "../Callbacks/AddMeshIntoSceneCallBack.h"
#include "../Commands/CmdProperties/CmdProperties.h"

#include <random>

class AddPlaneCallback : public Callback, public Observable, public Observer
{
public:
	virtual void execute(const Params& cmdParams) override
	{
		const AddPlaneParams& castedCmdParams = static_cast<const AddPlaneParams&>(cmdParams);
		Mesh* mesh = createPlaneVertexData(castedCmdParams);
		m_addedPlane = mesh;

		//CALL CALLBACK TO CREATE VAO DATA FO NEWLY ADDED MESH
		//ZAVOLAJ CALLBACK NA VYTVORENIE VAO DAT PRE NOVO PRIDANY MESH
		InitMeshVaoDataCallBack initMeshVaoDataCallBack;
		MeshParams meshParams;
		meshParams.m_mesh = mesh; //VYTVOR PARAMETRE KTORE BUDU POSLANE DO CALLBACKU
		initMeshVaoDataCallBack.execute(meshParams); //ZAVOLANIE CALLBACKU NA VYTVORENIE VAO DAT

		//CALL CALLBACK TO ADD NEWLY ADDED MESH INTO SCENE
		AddMeshIntoSceneCallBack addMeshIntoSceneCallBack;
		addMeshIntoSceneCallBack.execute(meshParams);
	}
private:

	Mesh* createPlaneVertexData(const AddPlaneParams& cmdParams)
	{
		++ViewPortsHolderContext::m_viewPortsHolder->m_currentMeshId;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(10.0f, 50.0f);

		float randomY = dis(gen);

		float planeSize = cmdParams.m_size;
		int planeSubidivisionLevel = cmdParams.m_subdivisionLevel;

		std::vector<std::vector<int>> planeIndices;
		std::vector<glm::vec3> planeVertices;

		float squareSize = planeSize / planeSubidivisionLevel;


		for (int z = 0; z <= planeSubidivisionLevel; ++z)
		{
			for (int x = 0; x <= planeSubidivisionLevel; ++x)
			{
				//VERTEX
				glm::vec3 vertex{ +(-(planeSize / 2)) + (x * squareSize) , 0.0f,  (-(planeSize / 2)) + (z * squareSize) };
				//0x -> (-(planeSize / 2)) + (x * squareSize);
				//0z -> (-(planeSize / 2)) + (z * squareSize);

				planeVertices.push_back(vertex);

				if (x != planeSubidivisionLevel && z != planeSubidivisionLevel)
				{
					//INDICES
					int firstIndex = (z * (planeSubidivisionLevel + 1)) + x;
					int secondIndex = ((z + 1) * (planeSubidivisionLevel + 1)) + x;
					int thirdIndex = (z * (planeSubidivisionLevel + 1)) + x + 1;
					int fifthIndex = ((z + 1) * (planeSubidivisionLevel + 1)) + x + 1;
					//0 -> (z * (planeSubidivisionLevel+1)) + x
					//5 -> ((z+1) * (planeSubidivisionLevel+1)) + x
					//1 -> (z * (planeSubidivisionLevel+1)) + x + 1
					//6 -> ((z+1) * (planeSubidivisionLevel+1)) + x + 1

					planeIndices.push_back({ firstIndex, secondIndex, thirdIndex });
					planeIndices.push_back({ fifthIndex, thirdIndex, secondIndex });
				}
			}
		}

		Mesh* mesh = new Mesh(new Material(ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_meshShader), new TriangleTriangulator(), planeIndices, planeVertices);

		mesh->m_meshID = std::to_string(ViewPortsHolderContext::m_viewPortsHolder->m_currentMeshId);
		return mesh;
	}

public:
	Mesh* m_addedPlane = nullptr;
};