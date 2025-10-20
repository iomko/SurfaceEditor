#pragma once
#include <vector>
#include "../IO/TiffGrayscaleMapLoader.h"
#include "../IO/CurlFileFetcher.h"
#include "../Utils/ThirdApiUtils.h"
#include <filesystem>

class FetchedSurfaceVertexGenCallable : public Callable<OpenTopoParams, MeshParams>
{
	void invoke(const OpenTopoParams& input, MeshParams& output) override
	{
		uint32_t width = 0, height = 0;

		std::string filePath = "heightmap.tif";
		CurlFileFetcher fileFetcher;
		fileFetcher.fetchFile(utils::third_api::buildOpenTopoUrl(input), filePath);

		float avgPixelHeight;
		std::vector<uint16_t> heightMap = TiffGrayscaleMapLoader::load16bit(filePath.c_str(), width, height, avgPixelHeight);

		std::error_code ec;
		std::filesystem::remove(filePath, ec);

		if (ec) {
			std::cerr << "Failed to delete temporary file: " << ec.message() << std::endl;
		}
		else {
			std::cout << "Temporary file deleted successfully." << std::endl;
		}

		std::vector<std::vector<int>> surfaceIndices;
		std::vector<glm::vec3> surfaceVertices;

		float verticesSpace = 2.0f;

		if (!heightMap.empty()) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					float heightInfo = static_cast<float>(heightMap[y * width + x]);

					glm::vec3 vertex;
					vertex.x = static_cast<float>(x) * verticesSpace;
					vertex.y = (heightInfo - avgPixelHeight) / 4.0f;
					vertex.z = static_cast<float>(y) * verticesSpace;
					surfaceVertices.emplace_back(vertex);

					if ((x != width - 1) && (y != height - 1))
					{
						int firstVertexIndex = (y * width) + x;
						int secondVertexIndex = (firstVertexIndex + width) + 1;
						int thirdVertexIndex = firstVertexIndex + width;
						int fourthVertexIndex = firstVertexIndex + 1;

						std::vector<int> firstTriangleIndices = { firstVertexIndex, thirdVertexIndex, secondVertexIndex };
						surfaceIndices.emplace_back(firstTriangleIndices);

						std::vector<int> secondTriangleIndices = { firstVertexIndex, secondVertexIndex, fourthVertexIndex };
						surfaceIndices.emplace_back(secondTriangleIndices);
					}

				}
			}
		}
       
        //Generate Mesh
        //Shader* meshShader = ViewPortsHolderContext::s_viewPortsController->m_viewPortLayers.at(0)->m_shaderSettings.m_meshShader;
		Mesh* mesh = new Mesh(surfaceIndices, surfaceVertices);
		output.m_mesh = mesh;
	}
};
