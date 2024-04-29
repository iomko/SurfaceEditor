#pragma once
#include "Callback.h"
#include "../Commands/TransformMeshCommand.h"

class TransformMeshCallBack : public Callback
{
public:
	TransformMeshCallBack(ViewPortHolder* viewPortHolder, TransformMeshCommand* transformMeshCommand)
	{
		m_viewPortHolder = viewPortHolder;
		m_transformMeshCommand = transformMeshCommand;
	}

	void applyModelMatrix(HalfEdgeDS::Vertex* vertex, const glm::mat4& modelMatrix) {
		glm::vec4 transformedPosition = modelMatrix * glm::vec4(vertex->getPosition(), 1.0f);

		vertex->setPosition(glm::vec3(transformedPosition));
	}


	virtual void execute() override
	{
		//v octree budem tento mesh uchovavat ako std::pair<Mesh*, Face*>
		std::map <Mesh*, std::map<HalfEdgeDS::Face*, std::vector<glm::vec3>>> m_meshesFaceOctreeMap;
		std::map <glm::vec3, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>> m_meshFaceOctreesMap;

		//v tomto mojom pripade staci len najst mesh v m_meshesFaceOctreeMap


		auto meshesFaceOctreeIt = m_meshesFaceOctreeMap.find(m_transformMeshCommand->m_transformedMesh);

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
						octreeIdOctreeIt->second.removeData(std::make_pair(m_transformMeshCommand->m_transformedMesh, currentFace));
						//je to vymazane z m_meshFaceOctreesMap
						//teraz sa pozriet ci je ten octree prazdny

						if (octreeIdOctreeIt->second.rootNode->dataCount == 0)
						{
							m_viewPortHolder->m_viewPortLayer->getScene()->m_meshFaceOctreesMap.erase(currentOctreeId);
						}
					}
				}

				//teraz tuto facu musime pridat naspat
				//na to aby sme ju mohli pridat naspat tak potrebujem vynasobit danu vertexu s mojou transformaciou

				
				m_transformMeshCommand->m_modelMatrix;

				//takze musim prechadzat cez vsetky vertices danej faci


				for (auto faceVerticesIt = currentFace->faceVertexBegin(); faceVerticesIt != currentFace->faceVertexEnd(); ++faceVerticesIt) {
					HalfEdgeDS::Vertex* currentVertex = &faceVerticesIt.operator*();

					//musim urobit transformaciu
					applyModelMatrix(currentVertex, m_transformMeshCommand->m_modelMatrix);
				}

				//teraz ta currentFace ma zmenene vertices

				//a uz ju len treba pridat



				Scene* scene = m_viewPortHolder->m_viewPortLayer->getScene();



				//potrebujem ziskat boundind box daneho facu

				auto& faceVerts = m_transformMeshCommand->m_transformedMesh->m_halfEdgeMesh->getVerticesFromFace(currentFace->getHalfEdge()->getFace());
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
							auto it = scene->m_meshFaceOctreesMap.find(currentIndexBound);
							if (it != scene->m_meshFaceOctreesMap.end())
							{
								//existuje octree s tymto indexom
								it->second.addDataToOctree(std::make_pair(m_transformMeshCommand->m_transformedMesh, &(*currentFace)), faceBounds);

							}
							else
							{
								//SceneUtilities::calculateOctreeBounds()
								//auto [octreeMinBound, octreeMaxBound] = scene->calculateOctreeBounds(currentIndexBound, scene->voxelXSize);
								auto [octreeMinBound, octreeMaxBound] = SceneUtilities::calculateOctreeBounds(currentIndexBound, scene);
								auto addedOctree = scene->m_meshFaceOctreesMap.emplace(currentIndexBound, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>(octreeMinBound, octreeMaxBound)).first;
								//neexistuje octree s tymto indexom
								addedOctree->second.addDataToOctree(std::make_pair(m_transformMeshCommand->m_transformedMesh, &(*currentFace)), faceBounds);

							}

							insertedFaceOctreeIdsMap.first->second.push_back(currentIndexBound);

						}
					}
				}








			}

		}



	}

private:
	ViewPortHolder* m_viewPortHolder;
	TransformMeshCommand* m_transformMeshCommand;
};