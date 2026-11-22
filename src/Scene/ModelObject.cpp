#include "ModelObject.h"

ModelObject::ModelObject()
{
    this->m_meshes = std::vector<Mesh*>();
    this->m_objectName = "DefaultObject";
}

ModelObject::ModelObject(std::string objectName)
{
    this->m_meshes = std::vector<Mesh*>();
    this->m_objectName = objectName;
}

ModelObject::~ModelObject()
{
    for (Mesh* mesh : m_meshes)
    {
        delete mesh;
    }
}

void ModelObject::addMesh(Mesh* mesh)
{
    if (mesh == nullptr)
    {
        return;
    }

    if (!containsMesh(mesh))
    {
        m_meshes.push_back(mesh);
    }
}

bool ModelObject::containsMesh(Mesh* mesh) const
{
    return std::find(m_meshes.begin(), m_meshes.end(), mesh) != m_meshes.end();
}

bool ModelObject::tryRemoveMesh(Mesh* mesh)
{
    auto it = std::find(m_meshes.begin(), m_meshes.end(), mesh);
    if (it != m_meshes.end())
    {
        m_meshes.erase(it);
        return true;
    }
    return false;
}

const std::vector<Mesh*>& ModelObject::getMeshes() const
{
    return m_meshes;
}

std::string ModelObject::getObjectName() const 
{
    return m_objectName;
}