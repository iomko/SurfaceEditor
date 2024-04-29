#pragma once
#include "Callback.h"

#include "../ViewPortHolder.h"
#include "../Commands/ImportMeshesCommand.h"

#include "../OBJImporter.h"
#include <random>

class ImportMeshesCallBack : public Callback, public Observable
{
public:
	ImportMeshesCallBack(ViewPortHolder* viewPortHolder, ImportMeshesCommand* importMeshesCommand)
	{
		m_viewPortHolder = viewPortHolder;
		m_importMeshesCommand = importMeshesCommand;
	}

	virtual void execute() override
	{
		//create Importer and read
		bool successfullMeshesRead = true;
		OBJImporter objImporter;
		if(objImporter.open(m_importMeshesCommand->m_filePathMeshes))
		{
			successfullMeshesRead = objImporter.read();
		}
		if(successfullMeshesRead)
		{
			for(const auto& importedMesh : objImporter.getMeshes())
			{
				Scene* scene = m_viewPortHolder->m_viewPortLayer->getScene();
				
				createMeshRenderingData(importedMesh);

				scene->m_meshesFaceOctreeMap.find(importedMesh);
				auto meshesFaceOctreeIt = scene->m_meshesFaceOctreeMap.find(importedMesh);

				if (meshesFaceOctreeIt == scene->m_meshesFaceOctreeMap.end()) {
					auto& faceOctreesMap = scene->m_meshesFaceOctreeMap[importedMesh];


					for (auto faceIter = importedMesh->m_halfEdgeMesh->faceIterBegin(); faceIter != importedMesh->m_halfEdgeMesh->faceIterEnd(); ++faceIter)
					{
						auto& faceVerts = importedMesh->m_halfEdgeMesh->getVerticesFromFace(faceIter);
						AABBBoundingRegion faceBounds(
							faceVerts.begin(),
							faceVerts.end(),
							[](HalfEdgeDS::Vertex& p) { return p.getPosition().x; },
							[](HalfEdgeDS::Vertex& p) { return p.getPosition().y; },
							[](HalfEdgeDS::Vertex& p) { return p.getPosition().z; }
						);

						//musime teraz vytvorit vsetky octrees alebo ak existuju octrees tak tam pridat tuto facu.

						//calculate the X,Y,Z for MinBound


						//glm::vec3 voxelIndexMinBound = SceneUtilities::getVoxelIndex(scene->voxelSize, faceBounds.getMin());
						glm::vec3 voxelIndexMinBound = SceneUtilities::getVoxelIndex(faceBounds.getMin(), scene);
						glm::vec3 voxelIndexMaxBound = SceneUtilities::getVoxelIndex(faceBounds.getMax(), scene);


						for (int x = voxelIndexMinBound.x; x <= voxelIndexMaxBound.x; ++x)
						{
							for (int y = voxelIndexMinBound.y; y <= voxelIndexMaxBound.y; ++y)
							{
								for (int z = voxelIndexMinBound.z; z <= voxelIndexMaxBound.z; ++z)
								{
									glm::vec3 currentIndexBound = { x,y,z };
									//teraz sme ziskali IndexBound pre facu. Teraz sa musime pozriet ci uz existuje octree s tymto indexom
									auto it = scene->m_meshFaceOctreesMap.find(currentIndexBound);
									if (it != scene->m_meshFaceOctreesMap.end())
									{
										//existuje octree s tymto indexom
										it->second.addDataToOctree(std::make_pair(importedMesh, &(*faceIter)), faceBounds);

									}
									else
									{
										//SceneUtilities::calculateOctreeBounds()
										//auto [octreeMinBound, octreeMaxBound] = scene->calculateOctreeBounds(currentIndexBound, scene->voxelXSize);
										auto [octreeMinBound, octreeMaxBound] = SceneUtilities::calculateOctreeBounds(currentIndexBound, scene);
										auto addedOctree = scene->m_meshFaceOctreesMap.emplace(currentIndexBound, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>(octreeMinBound, octreeMaxBound)).first;
										//neexistuje octree s tymto indexom
										addedOctree->second.addDataToOctree(std::make_pair(importedMesh, &(*faceIter)), faceBounds);

									}

									HalfEdgeDS::Face* facePointer = &(*faceIter);
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

	}
private:
	ViewPortHolder* m_viewPortHolder;
	ImportMeshesCommand* m_importMeshesCommand;

	void createMeshRenderingData(Mesh* mesh)
	{
		auto meshesShaderIt = m_viewPortHolder->m_meshesShaderData.find(mesh);

		ViewPortHolder::MeshRenderingFlags flags{ true, true, true, true };


		ViewPortHolder::MeshRenderingShaderData shaderData(*m_viewPortHolder->m_viewPortLayer->m_pointsShader, *m_viewPortHolder->m_viewPortLayer->m_linesShader,
			*m_viewPortHolder->m_viewPortLayer->m_normalsShader, *m_viewPortHolder->m_viewPortLayer->m_meshShader);

		ViewPortHolder::MeshRenderingVAOData vaoData;


		//edges
		for (auto& edge : mesh->m_halfEdgeMesh->m_edges)
		{
			glm::vec3 startPoint = (*edge.getFirstVertex()).getPosition();
			//startPoint.y += 0.005f;
			glm::vec3 endPoint = (*edge.getSecondVertex()).getPosition();
			//endPoint.y += 0.005f;


			if (meshesShaderIt == m_viewPortHolder->m_meshesShaderData.end()) {
				vaoData.m_edges.push_back({ startPoint, false });
				vaoData.m_edges.push_back({ endPoint, false });
			}

		}

		//vertices
		for (auto& vertex : mesh->m_halfEdgeMesh->m_vertices)
		{
			vaoData.m_points.push_back({ vertex.getPosition(), false });
		}


		if (meshesShaderIt == m_viewPortHolder->m_meshesShaderData.end())
		{
			m_viewPortHolder->m_meshesShaderData.emplace(mesh, std::make_tuple(flags, shaderData, vaoData));
		}
	}

};
