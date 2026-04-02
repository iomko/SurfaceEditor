#pragma once
#include "../../src/IO/BaseExporter.h"
#include "../../src/Utils/GeometryUtils.h"
#include <filesystem>
#include <fstream>

class OBJExporter : public BaseExporter
{
public:
	bool write() override
	{
		return writeMeshData();
	}

	void parseMesh(Mesh* mesh) override
	{
		m_meshes.push_back(mesh);
	}

	void setFilePath(const std::string& filePath)
	{
		m_filePath = filePath;
	}

private:
	bool writeMeshData()
	{
		if (m_meshes.empty()) {
			std::cerr << "Error: No meshes to export." << std::endl;
			return false;
		}

		std::ofstream outputFile(m_filePath);
		if (!outputFile.is_open()) {
			std::cerr << "Error: Failed to create or open file for writing: " << m_filePath << std::endl;
			return false;
		}

		std::cout << "File created: " << m_filePath << std::endl;

		std::ostringstream outputVectorStringStream;
		std::map<int, int> vertexIndexMap;
		std::ostringstream outputNormalStringStream;
		std::map<glm::vec3, int> normalIndexMap;
		std::ostringstream outputFaceStringStream;
		int currentVertexIndexInt = 1;
		int currentNormalIndexInt = 1;

		for (const auto& mesh : m_meshes)
		{
			outputFile << "o " << mesh->m_meshID << std::endl;

			// Write vertices
			for (const auto& vertex : mesh->m_halfEdgeStructure->m_vertices)
			{
				outputVectorStringStream << "v " << vertex->m_position.x << " "
					<< vertex->m_position.y << " "
					<< vertex->m_position.z << std::endl;
				vertexIndexMap[vertex->m_vertexIndexInVector] = currentVertexIndexInt;
				++currentVertexIndexInt;
			}

			// Iterate through all faces
			for (const auto& face : mesh->m_halfEdgeStructure->m_faces)
			{
				// Compute face normal
				glm::vec3 faceNormal = utils::geometry::computePolygonNormal(face);

				auto normalIndexIt = normalIndexMap.find(faceNormal);
				if (normalIndexIt == normalIndexMap.end()) {
					outputNormalStringStream << "vn " << faceNormal.x << " "
						<< faceNormal.y << " "
						<< faceNormal.z << std::endl;
					normalIndexMap[faceNormal] = currentNormalIndexInt;
					++currentNormalIndexInt;
				}

				outputFaceStringStream << "f ";
				for (auto faceVertexIt = face->faceVertexBegin(); faceVertexIt != face->faceVertexEnd(); ++faceVertexIt) {
				int vertexIndexInStringStream = vertexIndexMap[(*faceVertexIt).m_vertexIndexInVector];
					int normalIndexInStringStream = normalIndexMap[faceNormal];
					outputFaceStringStream << vertexIndexInStringStream << "/0/" << normalIndexInStringStream << " ";
				}
				outputFaceStringStream << std::endl;
			}

			// Write to file
			outputFile << outputVectorStringStream.str();
			outputFile << outputNormalStringStream.str();
			outputFile << outputFaceStringStream.str();

			// Clear buffers for next mesh
			vertexIndexMap.clear();
			normalIndexMap.clear();
			outputVectorStringStream.str("");
			outputNormalStringStream.str("");
			outputFaceStringStream.str("");
		}

		outputFile.close();
		
		return true;
	}

	std::string m_filePath;
	std::vector<Mesh*> m_meshes;
};