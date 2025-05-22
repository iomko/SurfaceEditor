#pragma once

class PlaneVertexGenCallable : public Callable<PlaneParams, MeshParams>
{
public:
	void invoke(const PlaneParams& input, MeshParams& output) override
	{
		float planeSize = input.m_size;
		int planeSubidivisionLevel = input.m_subdivisionLevel;

		std::vector<std::vector<int>> planeIndices;
		std::vector<glm::vec3> planeVertices;

		float squareSize = planeSize / planeSubidivisionLevel;

		for (int z = 0; z <= planeSubidivisionLevel; ++z)
		{
			for (int x = 0; x <= planeSubidivisionLevel; ++x)
			{
				glm::vec3 vertex{ (-(planeSize / 2)) + (x * squareSize) , 0.0f,  (-(planeSize / 2)) + (z * squareSize) };

				planeVertices.push_back(vertex);

				if (x != planeSubidivisionLevel && z != planeSubidivisionLevel)
				{
					int firstVertexIndex = (z * (planeSubidivisionLevel + 1)) + x;
					int secondVertexIndex = ((z + 1) * (planeSubidivisionLevel + 1)) + x;
					int thirdVertexIndex = (z * (planeSubidivisionLevel + 1)) + x + 1;
					int fourthVertexIndex = ((z + 1) * (planeSubidivisionLevel + 1)) + x + 1;

					planeIndices.push_back({ firstVertexIndex, secondVertexIndex, thirdVertexIndex });
					planeIndices.push_back({ fourthVertexIndex, thirdVertexIndex, secondVertexIndex });
				}
			}
		}

		Mesh* mesh = new Mesh(new Material(ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_meshShader), new TriangleTriangulator(), planeIndices, planeVertices);

		output.m_mesh = mesh; 
	}

private:

	/*
	Mesh* createNewTestMesh()
	{
		std::vector<std::vector<int>> planeIndices;
		std::vector<glm::vec3> planeVertices;

		planeVertices.emplace_back(glm::vec3(0.0f, -2.53614f, 0.0f)); //0 - index 0
		planeVertices.emplace_back(glm::vec3(0.0f, 11.6686f, -50.0f)); //1 - index 1
		planeVertices.emplace_back(glm::vec3(50.0f, 3.13265f, -50.0f)); //2 - index 2
		planeVertices.emplace_back(glm::vec3(50.0f, 1.31321f, 0.0f)); //3 - index 3

		std::vector<int> firstFace{ 0, 2, 1 };
		std::vector<int> secondFace{ 0, 3, 2 };

		planeIndices.emplace_back(firstFace);
		planeIndices.emplace_back(secondFace);

		Mesh* mesh = new Mesh(new Material(ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_meshShader), new TriangleTriangulator(), planeIndices, planeVertices);

		mesh->m_meshID = "testing_mesh";
		return mesh;
	}
	*/

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