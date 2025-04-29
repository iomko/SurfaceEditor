#pragma once
#include "../ViewPortsHolder.h"
#include "../Commands/CmdProperties/CmdProperties.h"
#include "../Callables/Callable.h"

class GenPlaneVertexDataCallable : public Callable<AddPlaneParams, MeshParams>
{
public:
	void invoke(const AddPlaneParams& input, MeshParams& output) override
	{
		++ViewPortsHolderContext::m_viewPortsHolder->m_currentMeshId;

		
		float planeSize = input.m_size;
		int planeSubidivisionLevel = input.m_subdivisionLevel;

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

		output.m_mesh = mesh; 
		

		//output.m_mesh = createTestingMesh();
	}

private:

	Mesh* createTestingMesh()
	{
		std::vector<std::vector<int>> planeIndices;
		std::vector<glm::vec3> planeVertices;

		planeVertices.emplace_back(glm::vec3(5.38461, 2.49756, 0.0)); //0 - index 0
		planeVertices.emplace_back(glm::vec3(10.7969, -6.18571, 0.0)); //1 - index 1
		planeVertices.emplace_back(glm::vec3(3.59558, -3.68423, 0.0)); //2 - index 2
		planeVertices.emplace_back(glm::vec3(-2.01404, -8.78555, 0.0)); //4 - index 3
		planeVertices.emplace_back(glm::vec3(-6.17731, -5.06843, 0.0)); //5 - index 4
		planeVertices.emplace_back(glm::vec3(1.60165, -1.69653, 0.0)); //3 - index 5
		planeVertices.emplace_back(glm::vec3(-8.15201, -2.79399, 0.0)); //6 - index 6
		planeVertices.emplace_back(glm::vec3(-6.77999, 0.257219, 0.0)); //7 - index 7
		planeVertices.emplace_back(glm::vec3(-1.93392, 2.98111, 0.0)); //8 - index 8

		std::vector<int> firstFace { 0, 1, 2, 3, 4, 5 };
		std::vector<int> secondFace { 8, 5, 4, 6, 7 };

		planeIndices.emplace_back(firstFace);
		planeIndices.emplace_back(secondFace);

		Mesh* mesh = new Mesh(new Material(ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_meshShader), new TriangleTriangulator(), planeIndices, planeVertices);

		mesh->m_meshID = "testing_mesh";
		return mesh;
	}
};