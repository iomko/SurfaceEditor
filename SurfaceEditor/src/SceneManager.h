#pragma once

import Scene;

class SceneManager
{
public:
    SceneManager(Scene& scene) : m_scene(scene) {}

    void addMesh(Mesh* mesh)
    {
        auto meshesFaceOctreeIt = m_scene.m_meshesFaceOctreeMap.find(mesh);

        if (meshesFaceOctreeIt == m_scene.m_meshesFaceOctreeMap.end()) {
            auto& faceOctreesMap = m_scene.m_meshesFaceOctreeMap[mesh];

            for (auto faceIter = mesh->m_halfEdgeMesh->faceIterBegin(); faceIter != mesh->m_halfEdgeMesh->faceIterEnd(); ++faceIter)
            {
                auto& faceVerts = mesh->m_halfEdgeMesh->getVerticesFromFace(faceIter);
                AABBBoundingRegion faceBounds(
                    faceVerts.begin(),
                    faceVerts.end(),
                    [](HalfEdgeDS::Vertex& p) { return p.getPosition().x; },
                    [](HalfEdgeDS::Vertex& p) { return p.getPosition().y; },
                    [](HalfEdgeDS::Vertex& p) { return p.getPosition().z; }
                );

                glm::vec3 sceneVoxelSize = m_scene.getVoxelSize();

                int voxelXIndexMinBound = m_scene.getVoxelIndex(sceneVoxelSize.x, faceBounds.getMin().x);
                int voxelYIndexMinBound = m_scene.getVoxelIndex(sceneVoxelSize.y, faceBounds.getMin().y);
                int voxelZIndexMinBound = m_scene.getVoxelIndex(sceneVoxelSize.z, faceBounds.getMin().z);

                int voxelXIndexMaxBound = m_scene.getVoxelIndex(sceneVoxelSize.x, faceBounds.getMax().x);
                int voxelYIndexMaxBound = m_scene.getVoxelIndex(sceneVoxelSize.y, faceBounds.getMax().y);
                int voxelZIndexMaxBound = m_scene.getVoxelIndex(sceneVoxelSize.z, faceBounds.getMax().z);

                for (int x = voxelXIndexMinBound; x <= voxelXIndexMaxBound; ++x)
                {
                    for (int y = voxelYIndexMinBound; y <= voxelYIndexMaxBound; ++y)
                    {
                        for (int z = voxelZIndexMinBound; z <= voxelZIndexMaxBound; ++z)
                        {
                            glm::vec3 currentIndexBound = { x,y,z };
                            auto it = m_scene.m_meshFaceOctreesMap.find(currentIndexBound);
                            if (it != m_scene.m_meshFaceOctreesMap.end())
                            {
                                it->second.addDataToOctree(std::make_pair(mesh, &(*faceIter)), faceBounds);
                            }
                            else
                            {
                                auto [octreeMinBound, octreeMaxBound] = m_scene.calculateOctreeBounds(currentIndexBound, voxelXSize);
                                auto addedOctree = m_scene.m_meshFaceOctreesMap.emplace(currentIndexBound, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>(octreeMinBound, octreeMaxBound)).first;
                                addedOctree->second.addDataToOctree(std::make_pair(mesh, &(*faceIter)), faceBounds);

                            }

                            HalfEdgeDS::Face* facePointer = &(*faceIter);
                            glm::vec3 octreeIndex = currentIndexBound;
                            faceOctreesMap[facePointer].push_back(octreeIndex);
                        }
                    }
                }


            }
        }
    }

private:
    Scene& m_scene;
};
