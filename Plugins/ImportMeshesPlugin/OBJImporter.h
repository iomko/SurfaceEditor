#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cctype>

#include "../../src/IO/BaseImporter.h"
class OBJImporter : public BaseImporter
{
public:

	void setFilePath(const std::string& filePath)
	{
		m_filePath = filePath;
	}

	bool read() override
	{
		if (!std::filesystem::exists(m_filePath)) {
			std::cerr << "Error: File does not exist: " << m_filePath << std::endl;
			return false;
		}

		std::cout << "File exists: " << m_filePath << std::endl;

		std::ifstream file(m_filePath);
		if (!file.is_open()) {
			std::cerr << "Error: Failed to open file: " << m_filePath << std::endl;
			return false;
		}

	
		auto trim = [](const std::string& s) {
			const auto ws_front = s.find_first_not_of(" \t\r\n");
			if (ws_front == std::string::npos) return std::string();
			const auto ws_back = s.find_last_not_of(" \t\r\n");
			return s.substr(ws_front, ws_back - ws_front + 1);
		};

		std::vector<glm::vec3> globalVertices;

		std::vector<glm::vec3> currentVertices;
		std::vector<std::vector<int>> currentPolygons;
		std::unordered_map<int,int> currentVertexRemap;

		auto flushCurrentMesh = [&]() -> bool {
			if (currentPolygons.empty()) {
				return true;
			}

			Mesh* mesh = new Mesh(currentPolygons, currentVertices);
			if (!mesh->isBuildSuccessful()) {
				delete mesh;
				return false;
			}

			// Calculate center of mesh bounds and set transform
			glm::vec3 minBounds = mesh->m_meshBounds.getMinBoundsPos();
			glm::vec3 maxBounds = mesh->m_meshBounds.getMaxBoundsPos();
			glm::vec3 center = (minBounds + maxBounds) * 0.5f;
			
			mesh->m_transform = glm::mat4(1.0f);
			mesh->m_transform[3] = glm::vec4(center, 1.0f);

			m_meshes.push_back(mesh);
			return true;
		};

		std::string rawLine;
		while (std::getline(file, rawLine)) {
			std::string line = trim(rawLine);
			if (line.empty() || line[0] == '#') continue;

			std::istringstream iss(line);
			std::string token;
			if (!(iss >> token)) continue;

			if (token == "o" || token == "g") {
				if (!flushCurrentMesh()) {
					file.close();
					return false;
				}
				currentVertices.clear();
				currentPolygons.clear();
				currentVertexRemap.clear();
				continue;
			}

			if (token == "v") {
				float x, y, z;
				if (!(iss >> x >> y >> z)) continue;
				globalVertices.emplace_back(x, y, z);
				continue;
			}

			if (token == "vn") {
				// Normals are parsed for completeness but not yet used by Mesh constructor.
				float nx, ny, nz;
				iss >> nx >> ny >> nz;
				continue;
			}

			if (token == "vt") {
				// Texture coordinates are ignored by current mesh import.
				float u, v;
				iss >> u >> v;
				continue;
			}

			if (token == "f") {
				std::vector<int> polygon;
				std::string faceToken;
				while (iss >> faceToken) {
					if (faceToken.empty()) continue;
					int vertexIndexGlobal = 0;
					{
						// face token formats: v, v/vt, v//vn, v/vt/vn
						size_t slash1 = faceToken.find('/');
						std::string vIndex = (slash1 == std::string::npos) ? faceToken : faceToken.substr(0, slash1);
						if (vIndex.empty()) continue;

						try {
							int idx = std::stoi(vIndex);
							if (idx < 0) {
								vertexIndexGlobal = static_cast<int>(globalVertices.size()) + idx;
							} else {
							vertexIndexGlobal = idx - 1;
						}
						} catch (...) {
							continue;
						}
					}

					if (vertexIndexGlobal < 0 || vertexIndexGlobal >= static_cast<int>(globalVertices.size())) {
						std::cerr << "Error: OBJ face vertex index out of range: " << faceToken << std::endl;
						continue;
					}

					auto mapIt = currentVertexRemap.find(vertexIndexGlobal);
					int localIndex;
					if (mapIt == currentVertexRemap.end()) {
						localIndex = static_cast<int>(currentVertices.size());
						currentVertices.push_back(globalVertices[vertexIndexGlobal]);
						currentVertexRemap[vertexIndexGlobal] = localIndex;
					} else {
						localIndex = mapIt->second;
					}

					polygon.push_back(localIndex);
				}

				if (polygon.size() >= 3) {
					currentPolygons.push_back(polygon);
				}
				continue;
			}

			// Ignored line types: usemtl, mtllib, s, etc.
		}

		if (!flushCurrentMesh()) {
			file.close();
			return false;
		}

		file.close();
		return !m_meshes.empty();
	}

	std::vector<Mesh*> getMeshes() override
	{
		return m_meshes;
	}

private:
	std::string m_filePath;

	std::vector<Mesh*> m_meshes;
};
