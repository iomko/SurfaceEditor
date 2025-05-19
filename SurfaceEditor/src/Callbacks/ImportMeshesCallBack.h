#pragma once
#include "Callback.h"
#include "../OBJImporter.h"
#include <random>


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

			Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;

			createMeshRenderingData(params, importedMesh);

			scene->meshFaceOctreeCoordsMap.find(importedMesh);
			auto meshesFaceOctreeIt = scene->meshFaceOctreeCoordsMap.find(importedMesh);

			if (meshesFaceOctreeIt == scene->meshFaceOctreeCoordsMap.end()) {
				auto& faceOctreesMap = scene->meshFaceOctreeCoordsMap[importedMesh];


				for (auto faceIter = importedMesh->m_halfEdgeStructure->faceIterBegin(); faceIter != importedMesh->m_halfEdgeStructure->faceIterEnd(); ++faceIter)
				{
					std::vector<HalfEdgeDS::Vertex> faceVerts;
					importedMesh->m_halfEdgeStructure->getVerticesFromFace(*faceIter, faceVerts);
					//auto& faceVerts = importedMesh->m_halfEdgeStructure->getVerticesFromFace(faceIter);
					AABBBoundingRegion faceBounds(
						faceVerts.begin(),
						faceVerts.end(),
						[](HalfEdgeDS::Vertex& p) { return p.m_position.x; },
						[](HalfEdgeDS::Vertex& p) { return p.m_position.y; },
						[](HalfEdgeDS::Vertex& p) { return p.m_position.z; }
					);

					//musime teraz vytvorit vsetky octrees alebo ak existuju octrees tak tam pridat tuto facu.

					//calculate the X,Y,Z for MinBound


					//glm::vec3 voxelIndexMinBound = SceneUtilities::getVoxelIndex(scene->voxelSize, faceBounds.getMin());
					glm::vec3 voxelIndexMinBound = SceneUtilities::getVoxelIndex(faceBounds.getMin(), scene->getVoxelSize());
					glm::vec3 voxelIndexMaxBound = SceneUtilities::getVoxelIndex(faceBounds.getMax(), scene->getVoxelSize());


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
									it->second.addDataToOctree(std::make_pair(importedMesh, *faceIter), faceBounds);

								}
								else
								{

									//SceneUtilities::calculateOctreeBounds()
									//auto [octreeMinBound, octreeMaxBound] = scene->calculateOctreeBounds(currentIndexBound, scene->voxelXSize);
									auto [octreeMinBound, octreeMaxBound] = SceneUtilities::calculateOctreeBounds(currentIndexBound, scene->getVoxelSize());
									auto addedOctree = scene->coordsOctreeMap.emplace(currentIndexBound, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>(octreeMinBound, octreeMaxBound)).first;
									//neexistuje octree s tymto indexom
									addedOctree->second.addDataToOctree(std::make_pair(importedMesh, *faceIter), faceBounds);

								}

								HalfEdgeDS::Face* facePointer = *faceIter;
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
			

			Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;
			
			createMeshRenderingData(castedCmdParams, importedMesh);

			scene->meshFaceOctreeCoordsMap.find(importedMesh);
			auto meshesFaceOctreeIt = scene->meshFaceOctreeCoordsMap.find(importedMesh);

			if (meshesFaceOctreeIt == scene->meshFaceOctreeCoordsMap.end()) {
				auto& faceOctreesMap = scene->meshFaceOctreeCoordsMap[importedMesh];


				for (auto faceIter = importedMesh->m_halfEdgeStructure->faceIterBegin(); faceIter != importedMesh->m_halfEdgeStructure->faceIterEnd(); ++faceIter)
				{
					std::vector<HalfEdgeDS::Vertex> faceVerts;
					importedMesh->m_halfEdgeStructure->getVerticesFromFace(faceIter, faceVerts);
					//auto& faceVerts = importedMesh->m_halfEdgeStructure->getVerticesFromFace(faceIter);
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
									auto addedOctree = scene->coordsOctreeMap.emplace(currentIndexBound, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>(octreeMinBound, octreeMaxBound)).first;
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
private:

	//ale toto nebudeme potrebovat
	//potrebujeme to nahradit niecim inym.

	//Mesh by v sebe nemal mat este navyse informacie o 
	//pripravi pre mesh rendering data potrebne na vykreslenie
	void createMeshRenderingData(const ImportExportMeshesParams& cmdParams, Mesh* mesh)
	{
		
		auto meshesShaderIt = ViewPortsHolderContext::m_viewPortsHolder->m_meshesShaderData.find(mesh);

		ViewPortHolder::MeshRenderingFlags flags{ true, true, true, true };

		

		ViewPortHolder::MeshRenderingShaderData shaderData(*ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayer->m_shaderSettings.m_pointsShader, *ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayer->m_shaderSettings.m_linesShader,
			*ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayer->m_shaderSettings.m_normalsShader, *ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayer->m_shaderSettings.m_meshShader);

		ViewPortHolder::MeshRenderingVAOData vaoData;


		//edges
		for (auto& edge : mesh->m_halfEdgeStructure->m_edges)
		{
			glm::vec3 startPoint = (*edge.getFirstVertex()).getPosition();
			//startPoint.y += 0.005f;
			glm::vec3 endPoint = (*edge.getSecondVertex()).getPosition();
			//endPoint.y += 0.005f;


			if (meshesShaderIt == ViewPortsHolderContext::m_viewPortsHolder->m_meshesShaderData.end()) {
				vaoData.m_edges.push_back({ startPoint, false });
				vaoData.m_edges.push_back({ endPoint, false });
			}

		}

		//vertices
		for (auto& vertex : mesh->m_halfEdgeStructure->m_vertices)
		{
			vaoData.m_points.push_back({ vertex.getPosition(), false });
		}


		if (meshesShaderIt == ViewPortsHolderContext::m_viewPortsHolder->m_meshesShaderData.end())
		{
			ViewPortsHolderContext::m_viewPortsHolder->m_meshesShaderData.emplace(mesh, std::make_tuple(flags, shaderData, vaoData));
		}

		
	}

};
*/
