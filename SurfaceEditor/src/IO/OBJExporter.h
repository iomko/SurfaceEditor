#pragma once
#include "BaseExporter.h"
#include <filesystem>

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
		/*
		/*
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
		std::map<HalfEdgeDS::VertexIndex, int> vertexIndexMap;
		std::ostringstream outputNormalStringStream;
		std::map<glm::vec3, int> normalIndexMap;
		std::ostringstream outputFaceStringStream;
		int currentVertexIndexInt = 1;
		int currentFaceIndexInt = 1;

		for (const auto& mesh : m_meshes)
		{
			outputFile << "o " << mesh->m_meshID << std::endl;

			// Write vertices
			for (auto& vertex : mesh->m_halfEdgeStructure->m_vertices)
			{
				outputVectorStringStream << "v " << vertex.getPosition().x << " "
					<< vertex.getPosition().y << " "
					<< vertex.getPosition().z << std::endl;
				vertexIndexMap[vertex.getHalfEdge()->getVertexIndex()] = currentVertexIndexInt;
				++currentVertexIndexInt;
			}

			// Write normals and faces


			//musime prechadzat cez vsetky meshes

			Scene* scene = ViewPortsHolderContext::s_viewPortsHolder->m_scene;

			//GET FACEINFO MAPPINGS
			SceneResources::MeshFacesMap::iterator meshFacesMapIt = scene->m_res.meshData.meshFacesMap.find(mesh);
			if (meshFacesMapIt != scene->m_res.meshData.meshFacesMap.end()) {
				SceneResources::FaceInfoMap& faceInfoMap = meshFacesMapIt->second;

				//ITERATE THROUGH ALL THE FACEINFO
				for (const auto& [faceIndex, faceInfo] : faceInfoMap) {

					auto normalIndexIt = normalIndexMap.find(faceInfo.normal);
					if (normalIndexIt == normalIndexMap.end()) {
						outputNormalStringStream << "vn " << faceInfo.normal.x << " "
							<< faceInfo.normal.y << " "
							<< faceInfo.normal.z << std::endl;
						normalIndexMap[faceInfo.normal] = currentFaceIndexInt;
						++currentFaceIndexInt;
					}

					HalfEdgeDS::Face& face = mesh->m_halfEdgeStructure->m_faces.at(faceIndex);

					outputFaceStringStream << "f ";
					for (auto faceVertexIt = face.faceVertexBegin(); faceVertexIt != face.faceVertexEnd(); ++faceVertexIt) {
						int vertexIndexInStringStream = vertexIndexMap[faceVertexIt.operator*().getHalfEdge()->getVertexIndex()];
						int normalIndexInStringStream = normalIndexMap[faceInfo.normal];
						outputFaceStringStream << vertexIndexInStringStream << "/0/" << normalIndexInStringStream << " ";
					}
					outputFaceStringStream << std::endl;

				}
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
		*/
		return true;
	}

	std::string m_filePath;
	std::vector<Mesh*> m_meshes;
};