#pragma once
#include "../IO/OBJImporter.h"

class ImportMeshesCallback : public Callback<ImportExportMeshesParams>, public Observer
{
public:
	virtual void execute(const ImportExportMeshesParams& params) override
	{
		
		OBJImporter objImporter;
		objImporter.setFilePath(params.m_filePathMeshes);
		objImporter.read();

		for (const auto& importedMesh : objImporter.getMeshes())
		{

			Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

			createMeshRenderingData(params, importedMesh);

			scene->m_res.meshFaceOctreeCoordsMap.find(importedMesh);
			auto meshesFaceOctreeIt = scene->m_res.meshFaceOctreeCoordsMap.find(importedMesh);

			if (meshesFaceOctreeIt == scene->m_res.meshFaceOctreeCoordsMap.end()) {
				auto& faceOctreesMap = scene->m_res.meshFaceOctreeCoordsMap[importedMesh];


				for (auto faceIter = importedMesh->m_halfEdgeStructure->faceIterBegin(); faceIter != importedMesh->m_halfEdgeStructure->faceIterEnd(); ++faceIter)
				{
					std::vector<ExtendedVertex> faceVerts;
					importedMesh->m_halfEdgeStructure->getVerticesFromFace(*faceIter, faceVerts);
					//auto& faceVerts = importedMesh->m_halfEdgeStructure->getVerticesFromFace(faceIter);

					AABBBoundingRegion faceBounds(faceVerts.begin(), faceVerts.end(),
						[](ExtendedVertex& point) { return point.m_position; });

					//musime teraz vytvorit vsetky octrees alebo ak existuju octrees tak tam pridat tuto facu.

					//calculate the X,Y,Z for MinBound


					//glm::vec3 voxelIndexMinBound = SceneUtilities::getVoxelIndex(scene->voxelSize, faceBounds.getMinBoundsPos());
					glm::vec3 voxelIndexMinBound = SceneUtilities::getVoxelIndex(faceBounds.getMinBoundsPos(), scene->getVoxelSize());
					glm::vec3 voxelIndexMaxBound = SceneUtilities::getVoxelIndex(faceBounds.getMaxBoundsPos(), scene->getVoxelSize());


					for (int x = voxelIndexMinBound.x; x <= voxelIndexMaxBound.x; ++x)
					{
						for (int y = voxelIndexMinBound.y; y <= voxelIndexMaxBound.y; ++y)
						{
							for (int z = voxelIndexMinBound.z; z <= voxelIndexMaxBound.z; ++z)
							{
								glm::vec3 currentIndexBound = { x,y,z };
								//teraz sme ziskali IndexBound pre facu. Teraz sa musime pozriet ci uz existuje octree s tymto indexom

								auto it = scene->m_res.coordsOctreeMap.find(currentIndexBound);
								if (it != scene->m_res.coordsOctreeMap.end())
								{
									//existuje octree s tymto indexom
									it->second.addDataToOctree(std::make_pair(importedMesh, *faceIter), faceBounds);

								}
								else
								{

									//SceneUtilities::calculateOctreeBounds()
									//auto [octreeMinBound, octreeMaxBound] = scene->calculateOctreeBounds(currentIndexBound, scene->voxelXSize);
									auto [octreeMinBound, octreeMaxBound] = SceneUtilities::calculateOctreeBounds(currentIndexBound, scene->getVoxelSize());
									auto addedOctree = scene->m_res.coordsOctreeMap.emplace(currentIndexBound, Octree<std::pair<Mesh*, ExtendedFace*>>(octreeMinBound, octreeMaxBound)).first;
									//neexistuje octree s tymto indexom
									addedOctree->second.addDataToOctree(std::make_pair(importedMesh, *faceIter), faceBounds);

								}

								ExtendedFace* facePointer = *faceIter;
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

private:
	void createMeshRenderingData(const ImportExportMeshesParams& cmdParams, Mesh* mesh)
	{

	}
};


/*
class ImportMeshesCallable : public Callback, public Observer
{
public:
	virtual void execute(const Params& cmdParams) override
	{
		const ImportExportMeshesParams& castedCmdParams = static_cast<const ImportExportMeshesParams&>(cmdParams);

		OBJImporter objImporter;
		objImporter.setFilePath(castedCmdParams.m_filePathMeshes);
		objImporter.read();
		
		for(const auto& importedMesh : objImporter.getMeshes())
		{
			

			Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
			
			createMeshRenderingData(castedCmdParams, importedMesh);

			scene->meshFaceOctreeCoordsMap.find(importedMesh);
			auto meshesFaceOctreeIt = scene->meshFaceOctreeCoordsMap.find(importedMesh);

			if (meshesFaceOctreeIt == scene->meshFaceOctreeCoordsMap.end()) {
				auto& faceOctreesMap = scene->meshFaceOctreeCoordsMap[importedMesh];


				for (auto faceIter = importedMesh->m_halfEdgeStructure->faceIterBegin(); faceIter != importedMesh->m_halfEdgeStructure->faceIterEnd(); ++faceIter)
				{
					std::vector<ExtendedVertex> faceVerts;
					importedMesh->m_halfEdgeStructure->getVerticesFromFace(faceIter, faceVerts);
					//auto& faceVerts = importedMesh->m_halfEdgeStructure->getVerticesFromFace(faceIter);
					AABBBoundingRegion faceBounds(
						faceVerts.begin(),
						faceVerts.end(),
						[](ExtendedVertex& p) { return p.getPosition().x; },
						[](ExtendedVertex& p) { return p.getPosition().y; },
						[](ExtendedVertex& p) { return p.getPosition().z; }
					);

					//musime teraz vytvorit vsetky octrees alebo ak existuju octrees tak tam pridat tuto facu.

					//calculate the X,Y,Z for MinBound


					//glm::vec3 voxelIndexMinBound = SceneUtilities::getVoxelIndex(scene->voxelSize, faceBounds.getMinBoundsPos());
					glm::vec3 voxelIndexMinBound = SceneUtilities::getVoxelIndex(faceBounds.getMinBoundsPos(), scene);
					glm::vec3 voxelIndexMaxBound = SceneUtilities::getVoxelIndex(faceBounds.getMaxBoundsPos(), scene);


					for (int x = voxelIndexMinBound.x; x <= voxelIndexMaxBound.x; ++x)
					{
						for (int y = voxelIndexMinBound.y; y <= voxelIndexMaxBound.y; ++y)
						{
							for (int z = voxelIndexMinBound.z; z <= voxelIndexMaxBound.z; ++z)
							{
								glm::vec3 currentIndexBound = { x,y,z };
								//teraz sme ziskali IndexBound pre facu. Teraz sa musime pozriet ci uz existuje octree s tymto indexom

								auto it = scene->coordsOctreeMap.find(currentIndexBound);
								if (it != scene->coordsOctreeMap.end())
								{
									//existuje octree s tymto indexom
									it->second.addDataToOctree(std::make_pair(importedMesh, &(*faceIter)), faceBounds);

								}
								else
								{
									//SceneUtilities::calculateOctreeBounds()
									//auto [octreeMinBound, octreeMaxBound] = scene->calculateOctreeBounds(currentIndexBound, scene->voxelXSize);
									auto [octreeMinBound, octreeMaxBound] = SceneUtilities::calculateOctreeBounds(currentIndexBound, scene);
									auto addedOctree = scene->coordsOctreeMap.emplace(currentIndexBound, Octree<std::pair<Mesh*, ExtendedFace*>>(octreeMinBound, octreeMaxBound)).first;
									//neexistuje octree s tymto indexom
									addedOctree->second.addDataToOctree(std::make_pair(importedMesh, &(*faceIter)), faceBounds);

								}

								ExtendedFace* facePointer = &(*faceIter);
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
private:

	//ale toto nebudeme potrebovat
	//potrebujeme to nahradit niecim inym.

	//Mesh by v sebe nemal mat este navyse informacie o 
	//pripravi pre mesh rendering data potrebne na vykreslenie
	void createMeshRenderingData(const ImportExportMeshesParams& cmdParams, Mesh* mesh)
	{
		
		auto meshesShaderIt = ViewPortsHolderContext::s_viewPortsController->m_meshesShaderData.find(mesh);

		ViewPortHolder::MeshRenderingFlags flags{ true, true, true, true };

		

		ViewPortHolder::MeshRenderingShaderData shaderData(*ViewPortsHolderContext::s_viewPortsController->m_viewPortLayer->m_shaderSettings.m_pointsShader, *ViewPortsHolderContext::s_viewPortsController->m_viewPortLayer->m_shaderSettings.m_linesShader,
			*ViewPortsHolderContext::s_viewPortsController->m_viewPortLayer->m_shaderSettings.m_normalsShader, *ViewPortsHolderContext::s_viewPortsController->m_viewPortLayer->m_shaderSettings.m_meshShader);

		ViewPortHolder::MeshRenderingVAOData vaoData;


		//edges
		for (auto& edge : mesh->m_halfEdgeStructure->m_edges)
		{
			glm::vec3 startPoint = (*edge.getFirstVertex()).getPosition();
			//startPoint.y += 0.005f;
			glm::vec3 endPoint = (*edge.getSecondVertex()).getPosition();
			//endPoint.y += 0.005f;


			if (meshesShaderIt == ViewPortsHolderContext::s_viewPortsController->m_meshesShaderData.end()) {
				vaoData.m_edges.push_back({ startPoint, false });
				vaoData.m_edges.push_back({ endPoint, false });
			}

		}

		//vertices
		for (auto& vertex : mesh->m_halfEdgeStructure->m_vertices)
		{
			vaoData.m_points.push_back({ vertex.getPosition(), false });
		}


		if (meshesShaderIt == ViewPortsHolderContext::s_viewPortsController->m_meshesShaderData.end())
		{
			ViewPortsHolderContext::s_viewPortsController->m_meshesShaderData.emplace(mesh, std::make_tuple(flags, shaderData, vaoData));
		}

		
	}

};
*/
