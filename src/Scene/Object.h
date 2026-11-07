#pragma once
#include "Mesh.h"

class Object
{
private:
    std::string m_objectName;
    std::vector<Mesh*> m_meshes;
public:
    Object();
    Object(std::string objectName);
    ~Object();  

    void addMesh(Mesh* mesh);
    bool containsMesh(Mesh* mesh) const;
    bool tryRemoveMesh(Mesh* mesh);
    const std::vector<Mesh*>& getMeshes() const;
    std::string getObjectName() const;
};