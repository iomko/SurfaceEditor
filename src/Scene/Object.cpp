#include "Object.h"

Object::Object()
{
    this->m_meshes = std::vector<Mesh*>();
    this->m_objectName = "DefaultObject";
}

Object::Object(std::string objectName)
{
    this->m_meshes = std::vector<Mesh*>();
    this->m_objectName = objectName;
}

Object::~Object()
{
    for (Mesh* mesh : m_meshes)
    {
        delete mesh;
    }
}

void Object::addMesh(Mesh* mesh)
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

bool Object::containsMesh(Mesh* mesh) const
{
    return std::find(m_meshes.begin(), m_meshes.end(), mesh) != m_meshes.end();
}

bool Object::tryRemoveMesh(Mesh* mesh)
{
    auto it = std::find(m_meshes.begin(), m_meshes.end(), mesh);
    if (it != m_meshes.end())
    {
        m_meshes.erase(it);
        return true;
    }
    return false;
}

const std::vector<Mesh*>& Object::getMeshes() const
{
    return m_meshes;
}

std::string Object::getObjectName() const 
{
    return m_objectName;
}