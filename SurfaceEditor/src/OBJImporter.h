#pragma once
#include <filesystem>

#include "BaseImporter.h"
class OBJImporter : public BaseImporter
{
public:
	bool open(const std::string& filePath) override
	{
		m_filePath = filePath;
		
		if (std::filesystem::exists(filePath)) {
			std::cout << "File exists: " << filePath << std::endl;
			return true;
		}
		else {
			std::cerr << "Error: File does not exist: " << filePath << std::endl;
			return false;
		}
	}
	bool read() override
	{

        std::vector<glm::vec3> vertices;
        std::vector<std::vector<int>> polygonsIndices;
        std::vector<glm::vec3> normals;
        std::vector<int> polygonNormalIndices;

        
        std::ifstream file(m_filePath);
        if (!file.is_open()) {
            std::cerr << "Error: Failed to open file: " << m_filePath << std::endl;
            return false;
        }

        bool isFirstAddedMesh = true;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            if (line[0] == 'o') {
                

                if(isFirstAddedMesh)
                {
                    isFirstAddedMesh = false;
                } else
                {
	                
                    Mesh* addedMesh = new Mesh(new Material(line.substr(2)), polygonsIndices, vertices, polygonNormalIndices, normals);
                    if(addedMesh->isBuildSuccessful())
                    {
                        m_meshes.push_back(addedMesh);
                    } else
                    {
                        delete addedMesh;

                        return false;
                    }

                    //vertices.clear();
                    //normals.clear();
                }
            }
            else if (line[0] == 'v') {
                // Extract vertex or normal data
                std::vector<float> values;
                size_t start = line.find(' ') + 1;
                size_t end;

                
                while ((end = line.find(' ', start)) != std::string::npos) {
                    values.push_back(std::stof(line.substr(start, end - start)));
                    start = end + 1;
                }

                if (start < line.length()) {
                    values.push_back(std::stof(line.substr(start)));
                }

                glm::vec3 vec3Value = glm::vec3(values[0], values[1], values[2]);

                if (line[1] == ' ') {
                    vertices.push_back(vec3Value);
                }
                else if (line[1] == 'n') {
                    normals.push_back(vec3Value);
                }

                
            }
            else if (line[0] == 'f') {
                int vertexIndex, vertexTexCoord, vertexNormal;
                
                std::vector<int> polygonIndices;

                size_t start = line.find(' ') + 1;
                size_t end;
                std::cout << "line: " << line << std::endl;
                while ((end = line.find(' ', start)) != std::string::npos) {
                    std::string faceData = line.substr(start, end - start);
                    size_t firstSlash = faceData.find('/');
                    size_t secondSlash = faceData.find('/', firstSlash + 1);

                    if (secondSlash != std::string::npos && secondSlash != firstSlash + 1) {
                        // Format: vertexIndex/textureIndex/normalIndex
                        vertexIndex = std::stoi(faceData.substr(0, firstSlash));
                        vertexTexCoord = std::stoi(faceData.substr(firstSlash + 1, secondSlash - firstSlash - 1));
                        vertexNormal = std::stoi(faceData.substr(secondSlash + 1));
                    }
                    else {
                        // Format: vertexIndex//normalIndex
                        vertexIndex = std::stoi(faceData.substr(0, firstSlash));
                        vertexTexCoord = 0;
                        vertexNormal = std::stoi(faceData.substr(firstSlash + 2));
                    }

                    std::cout << "First number: " << vertexIndex << ", Second number: " << vertexTexCoord << ", Third number: " << vertexNormal << std::endl;

                    std::cout << "start: " << start << ", end: " << end << std::endl;

                    polygonIndices.push_back(vertexIndex-1);

                    start = end + 1;
                }

              
                std::string lastFaceData = line.substr(start);

                bool containsOnlyWhitespace = std::all_of(lastFaceData.begin(), lastFaceData.end(), [](unsigned char c) {
                    return std::isspace(c);
                    });

                if(!containsOnlyWhitespace)
                {
                    size_t firstSlash = lastFaceData.find('/');
                    size_t secondSlash = lastFaceData.find('/', firstSlash + 1);

                    std::cout << "Last face data: " << lastFaceData << std::endl;
                    std::cout << "First slash: " << firstSlash << ", Second slash: " << secondSlash << std::endl;

                    if (secondSlash != std::string::npos && secondSlash != firstSlash + 1) {
                        vertexIndex = std::stoi(lastFaceData.substr(0, firstSlash));
                        vertexTexCoord = std::stoi(lastFaceData.substr(firstSlash + 1, secondSlash - firstSlash - 1));
                        vertexNormal = std::stoi(lastFaceData.substr(secondSlash + 1));
                    }
                    else {
                        std::cout << "firstSlash: " << firstSlash << std::endl;
                        vertexIndex = std::stoi(lastFaceData.substr(0, firstSlash));
                        vertexTexCoord = 0;
                        vertexNormal = std::stoi(lastFaceData.substr(firstSlash + 2));
                    }

                    std::cout << "First number: " << vertexIndex << ", Second number: " << vertexTexCoord << ", Third number: " << vertexNormal << std::endl;

                    polygonIndices.push_back(vertexIndex - 1);
                    polygonsIndices.push_back(polygonIndices);
                    polygonNormalIndices.push_back(vertexNormal - 1);
                } else
                {
                    polygonsIndices.push_back(polygonIndices);
                    polygonNormalIndices.push_back(vertexNormal - 1);
                }
            }
        }

        Mesh* addedMesh = new Mesh(new Material("1"), polygonsIndices, vertices, polygonNormalIndices, normals);

        if (addedMesh->isBuildSuccessful())
        {
            m_meshes.push_back(addedMesh);
        }
        else
        {
            delete addedMesh;
            return false;
        }

        file.close();
        return true;
	}
	std::vector<Mesh*> getMeshes() override
	{
		return m_meshes;
	}

private:
	std::string m_filePath;

	std::vector<Mesh*> m_meshes;
};