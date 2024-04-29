#pragma once
#include "BaseExporter.h"
#include <filesystem>
class OBJExporter : public BaseExporter
{
public:
	bool open(const std::string& filePath) override
	{
		m_filePath = filePath;
		std::ofstream file(m_filePath);

		if (file.is_open()) {
			std::cout << "File created: " << filePath << std::endl;
			file.close();
			return true;
		}
		else {
			std::cerr << "Error: Failed to create file: " << filePath << std::endl;
			return false;
		}
	}

	bool write() override
	{
		return writeMeshData();
	}

	void setMesh(Mesh* mesh) override
	{
		m_meshes.push_back(mesh);
	}

	bool writeMeshData()
	{
		std::ofstream outputFile(m_filePath, std::ios::app);
		if (!outputFile.is_open()) {
			std::cerr << "Error: Failed to open output file for writing: " << m_filePath << std::endl;
			return false;
		}
		std::ostringstream outputVectorStringStream;
		std::map<HalfEdgeDS::VertexIndex, int> vertexIndexMap;
		std::ostringstream outputNormalStringStream;
		std::map<glm::vec3, int> normalIndexMap;
		std::ostringstream outputFaceStringStream;
		int currentVertexIndexInt = 1;
		int currentFaceIndexInt = 1;
		for (const auto& mesh : m_meshes)
		{
			for (auto& vertex : mesh->m_halfEdgeMesh->m_vertices)
			{
				outputVectorStringStream << "v " << vertex.getPosition().x << " " << vertex.getPosition().y << " " << vertex.getPosition().z << std::endl;
				vertexIndexMap[vertex.getHalfEdge()->getVertexIndex()] = currentVertexIndexInt;
				++currentVertexIndexInt;
			}
			outputFile << "o " << mesh->m_meshID << std::endl;
			for (const auto& faceData : mesh->m_facesData)
			{
				auto normalIndexIt = normalIndexMap.find(faceData.normal);
				if (normalIndexIt == normalIndexMap.end()) {
					outputNormalStringStream << "vn " << faceData.normal.x << " " << faceData.normal.y << " " << faceData.normal.z << std::endl;
					normalIndexMap.insert(std::make_pair(faceData.normal, currentFaceIndexInt));
					++currentFaceIndexInt;
				}
				HalfEdgeDS::Face& currentFace = mesh->m_halfEdgeMesh->m_faces.at(faceData.faceIndex);
				outputFaceStringStream << "f ";
				for (auto faceVertexIt = currentFace.faceVertexBegin(); faceVertexIt != currentFace.faceVertexEnd(); ++faceVertexIt) {
					int vertexIndexInStringStream = vertexIndexMap[faceVertexIt.operator*().getHalfEdge()->getVertexIndex()];
					int normalIndexInStringStream = normalIndexMap[faceData.normal];
					outputFaceStringStream << vertexIndexInStringStream << "/0/" << normalIndexInStringStream << " ";
				}
				outputFaceStringStream << std::endl;
			}
			outputFile << outputVectorStringStream.str();
			outputFile << outputNormalStringStream.str();
			outputFile << outputFaceStringStream.str();
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