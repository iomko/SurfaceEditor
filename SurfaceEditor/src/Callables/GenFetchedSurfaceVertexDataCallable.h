#pragma once
#include <cstdint>
#include <vector>
#include "../ViewPortsHolder.h"
#include "../TiffGrayscaleMapLoader.h"
#include "../CurlFileFetcher.h"
#include "../Commands/CmdProperties/CmdProperties.h"
#include "../Callables/Callable.h"
#include <filesystem>

class GenFetchedSurfaceVertexDataCallable : public Callable<FetchSurfaceParams, MeshParams>
{
	void invoke(const FetchSurfaceParams& input, MeshParams& output) override
	{
		++ViewPortsHolderContext::m_viewPortsHolder->m_currentMeshId;

		uint32_t width = 0, height = 0;

		std::string filePath = "heightmap.tif";
		CurlFileFetcher fileFetcher;
		fileFetcher.fetchFile(buildUrl(input), filePath);

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

		std::vector<std::vector<int>> indices;
		std::vector<glm::vec3> vertices;

		//space between vectors
		float space = 2.0f;

		if (!heightMap.empty()) {
			std::cout << "Loaded heightmap: " << width << "x" << height << std::endl;

			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					float heightInfo = static_cast<float>(heightMap[y * width + x]);

					glm::vec3 vertex;
					vertex.x = (float)x * space;
					vertex.y = (heightInfo - avgPixelHeight) / 4.0f;
					vertex.z = (float)y * space;
					vertices.emplace_back(vertex);

					if ((x != width - 1) && (y != height - 1))
					{
						int firstVertexIndex = (y * width) + x;
						int secondVertexIndex = (firstVertexIndex + width) + 1;
						int thirdVertexIndex = firstVertexIndex + width;
						int fourthVertexIndex = firstVertexIndex + 1;

						//first_triangle
						std::vector<int> firstTriangleIndices = { firstVertexIndex, secondVertexIndex, thirdVertexIndex };
						indices.emplace_back(firstTriangleIndices);

						//second_triangle
						std::vector<int> secondTriangleIndices = { firstVertexIndex, fourthVertexIndex, secondVertexIndex };
						indices.emplace_back(secondTriangleIndices);
					}

				}
			}
		}

		Mesh* mesh = new Mesh(new Material(ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_meshShader), new TriangleTriangulator(), indices, vertices);

		mesh->m_meshID = std::to_string(ViewPortsHolderContext::m_viewPortsHolder->m_currentMeshId);

		output.m_mesh = mesh;
	}

private:
	std::string buildUrl(const FetchSurfaceParams& input)
	{
		std::ostringstream oss;
		oss << "https://portal.opentopography.org/API/globaldem?"
			<< "demtype=NASADEM"
			<< "&south=" << input.m_lowerLeftLat
			<< "&north=" << input.m_upperRightLat
			<< "&west=" << input.m_lowerLeftLon
			<< "&east=" << input.m_upperRightLon
			<< "&outputFormat=GTiff"
			<< "&API_Key=" << input.m_apiKey;

		return oss.str();
	}
};