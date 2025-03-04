#pragma once
#include "Callback.h"
#include "../Commands/TransformMeshCommand.h"

class TransformMeshCallBack : public Callback
{
public:
	void applyModelMatrix(HalfEdgeDS::Vertex* vertex, const glm::mat4& modelMatrix) {
		glm::vec4 transformedPosition = modelMatrix * glm::vec4(vertex->getPosition(), 1.0f);

		vertex->setPosition(glm::vec3(transformedPosition));
	}

	virtual void execute(const Params& cmdParams) override
	{
		const TransformMeshParams& castedCmdParams = static_cast<const TransformMeshParams&>(cmdParams);

		//v octree budem tento mesh uchovavat ako std::pair<Mesh*, Face*>
		std::map <Mesh*, std::map<HalfEdgeDS::Face*, std::vector<glm::vec3>>> m_meshesFaceOctreeMap;
		std::map <glm::vec3, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>> m_meshFaceOctreesMap;

		//v tomto mojom pripade staci len najst mesh v m_meshesFaceOctreeMap


		auto meshesFaceOctreeIt = m_meshesFaceOctreeMap.find(castedCmdParams.m_transformedMesh);

		if(meshesFaceOctreeIt != m_meshesFaceOctreeMap.end())
		{
			//musim prechadzat cez vsetky faces

			for (const auto& faceOctreesIdIt : meshesFaceOctreeIt->second)
			{
				HalfEdgeDS::Face* currentFace = faceOctreesIdIt.first;
				const auto& currentOctreesId = faceOctreesIdIt.second;

				for (const auto& currentOctreeId : currentOctreesId)
				{
					//musime najst akemu octree pripada dane idcko
					auto octreeIdOctreeIt = m_meshFaceOctreesMap.find(currentOctreeId);
					if(octreeIdOctreeIt != m_meshFaceOctreesMap.end())
					{
						octreeIdOctreeIt->second.removeData(std::make_pair(castedCmdParams.m_transformedMesh, currentFace));
						//je to vymazane z m_meshFaceOctreesMap
						//teraz sa pozriet ci je ten octree prazdny

						if (octreeIdOctreeIt->second.rootNode->dataCount == 0)
						{
							ViewPortsHolderContext::m_viewPortsHolder->m_scene->coordsOctreeMap.erase(currentOctreeId);
						}
					}
				}

				//teraz tuto facu musime pridat naspat
				//na to aby sme ju mohli pridat naspat tak potrebujem vynasobit danu vertexu s mojou transformaciou

				
				castedCmdParams.m_modelMatrix;

				//takze musim prechadzat cez vsetky vertices danej faci


				for (auto faceVerticesIt = currentFace->faceVertexBegin(); faceVerticesIt != currentFace->faceVertexEnd(); ++faceVerticesIt) {
					HalfEdgeDS::Vertex* currentVertex = &faceVerticesIt.operator*();

					//musim urobit transformaciu
					
					applyModelMatrix(currentVertex, castedCmdParams.m_modelMatrix);
				}

				//teraz ta currentFace ma zmenene vertices

				//a uz ju len treba pridat

				Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;


				//potrebujem ziskat boundind box daneho facu

				std::vector<HalfEdgeDS::Vertex> faceVerts;
				castedCmdParams.m_transformedMesh->m_halfEdgeStructure->getVerticesFromFace(currentFace->getHalfEdge()->getFace(), faceVerts);
				//auto& faceVerts = castedCmdParams.m_transformedMesh->m_halfEdgeStructure->getVerticesFromFace(currentFace->getHalfEdge()->getFace());
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


		
				
				auto insertedFaceOctreeIdsMap = meshesFaceOctreeIt->second.insert(std::make_pair(currentFace, std::vector<glm::vec3>{}));


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
								it->second.addDataToOctree(std::make_pair(castedCmdParams.m_transformedMesh, &(*currentFace)), faceBounds);

							}
							else
							{
								//SceneUtilities::calculateOctreeBounds()
								//auto [octreeMinBound, octreeMaxBound] = scene->calculateOctreeBounds(currentIndexBound, scene->voxelXSize);
								auto [octreeMinBound, octreeMaxBound] = SceneUtilities::calculateOctreeBounds(currentIndexBound, scene);
								auto addedOctree = scene->coordsOctreeMap.emplace(currentIndexBound, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>(octreeMinBound, octreeMaxBound)).first;
								//neexistuje octree s tymto indexom
								addedOctree->second.addDataToOctree(std::make_pair(castedCmdParams.m_transformedMesh, &(*currentFace)), faceBounds);

							}

							insertedFaceOctreeIdsMap.first->second.push_back(currentIndexBound);

						}
					}
				}

			}

		}



	}
};