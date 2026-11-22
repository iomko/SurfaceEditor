#pragma once

#include "Mesh.h"
class Mesh;
class ModelObject
{
private:
    std::string m_objectName;
    std::vector<Mesh*> m_meshes;
public:
    ModelObject();
    ModelObject(std::string objectName);
    ~ModelObject();  

    void addMesh(Mesh* mesh);
    bool containsMesh(Mesh* mesh) const;
    bool tryRemoveMesh(Mesh* mesh);
    const std::vector<Mesh*>& getMeshes() const;
    std::string getObjectName() const;
};