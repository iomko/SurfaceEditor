#include "MeshSelectionManager.h"

void MeshSelectionManager::registerMesh(Mesh* mesh)
{
    if (mesh != nullptr && !mesh->m_selected)
    {
        m_holder.meshes.emplace_back(mesh);
        mesh->m_selected = true;
        mesh->m_selectionIndex = m_holder.meshes.size() - 1;

        highlight(mesh);
    }
}

void MeshSelectionManager::unregisterMesh(Mesh* mesh)
{
    if (mesh != nullptr && mesh->m_selected)
    {
        int indexInSelection = mesh->m_selectionIndex;
        std::vector<Mesh*>& selectionVector = m_holder.meshes;
        if (indexInSelection != selectionVector.size() - 1)
        {
            // vymenime entry s poslednym vo vectore

            Mesh* lastMesh = selectionVector.back();
            selectionVector.at(indexInSelection) = lastMesh;

            lastMesh->m_selectionIndex = indexInSelection;

            selectionVector.back() = mesh;
        }
        mesh->m_selected = false;
        mesh->m_selectionIndex = -1;
        selectionVector.erase(selectionVector.end() - 1);

        unhighlight(mesh);

        // selected faces nemusia existovat
        auto faceSelectionsIt = m_holder.faces.find(mesh);
        if (faceSelectionsIt != m_holder.faces.end())
        {
            // vymaz vsetky selected faces selectnuteho meshu
            std::vector<ExtendedFace*>& faceSelections = m_holder.faces.find(mesh)->second;

            while (!faceSelections.empty())
            {
                ExtendedFace* face = faceSelections.back();

                face->m_selected = false;
                face->m_selectionIndex = -1;

                faceSelections.pop_back();
            }

            m_holder.faces.erase(mesh);
        }
    }
}

void MeshSelectionManager::highlight(Mesh* mesh)
{
    for (auto it = mesh->bufferLayout.triangleBuffersBegin();
             it != mesh->bufferLayout.triangleBuffersEnd(); ++it)
    {
        TriangleBufferStorage& triangleBufferStorage = it->second;
        auto& vertices = triangleBufferStorage.data.vertices;

        for (auto& v : vertices)
        {
            v.isHighlited = 1.0f;
        }

        triangleBufferStorage.update();
    }
}

void MeshSelectionManager::unhighlight(Mesh* mesh)
{
    for (auto it = mesh->bufferLayout.triangleBuffersBegin();
             it != mesh->bufferLayout.triangleBuffersEnd(); ++it)
    {
        TriangleBufferStorage& triangleBufferStorage = it->second;
        auto& vertices = triangleBufferStorage.data.vertices;

        for (auto& v : vertices)
        {
            v.isHighlited = 0.0f;
        }

        triangleBufferStorage.update();
    }
}