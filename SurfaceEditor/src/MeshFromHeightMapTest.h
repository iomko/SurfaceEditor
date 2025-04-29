#pragma once
#include <iostream>
#include <vector>
#include <glm/vec3.hpp>

class MeshFromHeightMapTest
{
public:
	static void test(int width, int height, const std::vector<uint16_t>& heightMap, 
		std::vector<std::vector<int>>& outIndices, std::vector<glm::vec3>& outVertices)
	{
		//space between vectors
		float space = 2.0f;

		if (!heightMap.empty()) {
			std::cout << "Loaded heightmap: " << width << "x" << height << std::endl;

			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					float heightInfo = static_cast<float>(heightMap[y * width + x]);

					glm::vec3 vertex;
					vertex.x = (float)x * space;
					vertex.y = heightInfo / 4.0f;
					vertex.z = (float)y * space;
					outVertices.emplace_back(vertex);

					if((x != width-1) && (y != height-1))
					{
						//firstVertexIndex
						int firstVertexIndex = (y * width) + x;
						int secondVertexIndex = (firstVertexIndex + width) + 1;
						int thirdVertexIndex = firstVertexIndex + width;
						int fourthVertexIndex = firstVertexIndex + 1;

						//first_triangle
						std::vector<int> firstTriangleIndices = {firstVertexIndex, secondVertexIndex, thirdVertexIndex};
						outIndices.emplace_back(firstTriangleIndices);

						//second_triangle
						std::vector<int> secondTriangleIndices = { firstVertexIndex, fourthVertexIndex, secondVertexIndex };
						outIndices.emplace_back(secondTriangleIndices);
					}
					
				}
			}
		}

	}
};
