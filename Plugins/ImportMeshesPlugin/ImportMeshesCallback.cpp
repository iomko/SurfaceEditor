#include "ImportMeshesCallBack.h"
#include "../../src/ViewPortsController.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "OBJImporter.h"

static AutoRegisterCallback<ImportMeshesCallback> autoRegister("IMPORT_MESHES_CALLBACK");

void ImportMeshesCallback::execute(const ImportExportMeshesParams &params)
{
    OBJImporter objImporter;
    objImporter.setFilePath(params.m_filePathMeshes);
    objImporter.read();

    for (const auto &importedMesh : objImporter.getMeshes())
    {
        Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

        createMeshRenderingData(params, importedMesh);

        scene->m_res.meshFaceOctreeCoordsMap.find(importedMesh);
        auto meshesFaceOctreeIt = scene->m_res.meshFaceOctreeCoordsMap.find(importedMesh);

        if (meshesFaceOctreeIt == scene->m_res.meshFaceOctreeCoordsMap.end())
        {
            auto &faceOctreesMap = scene->m_res.meshFaceOctreeCoordsMap[importedMesh];

            for (auto faceIter = importedMesh->m_halfEdgeStructure->faceIterBegin(); faceIter != importedMesh->m_halfEdgeStructure->faceIterEnd(); ++faceIter)
            {
                std::vector<ExtendedVertex> faceVerts;
                importedMesh->m_halfEdgeStructure->getVerticesFromFace(*faceIter, faceVerts);
                // auto& faceVerts = importedMesh->m_halfEdgeStructure->getVerticesFromFace(faceIter);

                AABBBoundingRegion faceBounds(faceVerts.begin(), faceVerts.end(),
                                              [](ExtendedVertex &point)
                                              { return point.m_position; });

                // calculate the X,Y,Z for MinBound

                // glm::vec3 voxelIndexMinBound = SceneUtilities::getVoxelIndex(scene->voxelSize, faceBounds.getMinBoundsPos());
                glm::vec3 voxelIndexMinBound = SceneUtilities::getVoxelIndex(faceBounds.getMinBoundsPos(), scene->getVoxelSize());
                glm::vec3 voxelIndexMaxBound = SceneUtilities::getVoxelIndex(faceBounds.getMaxBoundsPos(), scene->getVoxelSize());

                for (int x = voxelIndexMinBound.x; x <= voxelIndexMaxBound.x; ++x)
                {
                    for (int y = voxelIndexMinBound.y; y <= voxelIndexMaxBound.y; ++y)
                    {
                        for (int z = voxelIndexMinBound.z; z <= voxelIndexMaxBound.z; ++z)
                        {
                            glm::vec3 currentIndexBound = {x, y, z};

                            auto it = scene->m_res.coordsOctreeMap.find(currentIndexBound);
                            if (it != scene->m_res.coordsOctreeMap.end())
                            {
                                it->second.addDataToOctree(std::make_pair(importedMesh, *faceIter), faceBounds);
                            }
                            else
                            {

                                // SceneUtilities::calculateOctreeBounds()
                                // auto [octreeMinBound, octreeMaxBound] = scene->calculateOctreeBounds(currentIndexBound, scene->voxelXSize);
                                auto [octreeMinBound, octreeMaxBound] = SceneUtilities::calculateOctreeBounds(currentIndexBound, scene->getVoxelSize());
                                auto addedOctree = scene->m_res.coordsOctreeMap.emplace(currentIndexBound, Octree<std::pair<Mesh *, ExtendedFace *>>(octreeMinBound, octreeMaxBound)).first;
                                addedOctree->second.addDataToOctree(std::make_pair(importedMesh, *faceIter), faceBounds);
                            }

                            ExtendedFace *facePointer = *faceIter;
                            glm::vec3 octreeIndex = currentIndexBound;
                            // Now add the entry to the inner map
                            faceOctreesMap[facePointer].push_back(octreeIndex);
                        }
                    }
                }
            }
        }
    }
}
void ImportMeshesCallback::createMeshRenderingData(const ImportExportMeshesParams& cmdParams, Mesh* mesh){}
