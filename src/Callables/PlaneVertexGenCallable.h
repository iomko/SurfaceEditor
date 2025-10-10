#pragma once
#include "../Renderer/MaterialRegistry.h"

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
        
        //get proper material
		Mesh* mesh = new Mesh(planeIndices, planeVertices);

		output.m_mesh = mesh; 
	}
};
