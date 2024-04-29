#pragma once
#include "Callback.h"
#include "../Commands/AddPlaneCommand.h"

#include "../ViewPortHolder.h"

#include <random>

class AddPlaneCallback : public Callback, public Observable
{
public:
	AddPlaneCallback(ViewPortHolder* viewPortHolder, AddPlaneCommand* command)
	{
		m_viewPortHolder = viewPortHolder;
		m_command = command;
	}

	virtual void execute() override
	{
        Mesh* mesh = createPlaneVertexData();
		createPlaneRenderingData(mesh);

		Scene* scene = m_viewPortHolder->m_viewPortLayer->getScene();
		
		
	    scene->m_meshesFaceOctreeMap.find(mesh);
	    auto meshesFaceOctreeIt = scene->m_meshesFaceOctreeMap.find(mesh);

		if (meshesFaceOctreeIt == scene->m_meshesFaceOctreeMap.end()) {
			auto& faceOctreesMap = scene->m_meshesFaceOctreeMap[mesh];


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
								it->second.addDataToOctree(std::make_pair(mesh, &(*faceIter)), faceBounds);

							}
							else
							{
								//SceneUtilities::calculateOctreeBounds()
								//auto [octreeMinBound, octreeMaxBound] = scene->calculateOctreeBounds(currentIndexBound, scene->voxelXSize);
								auto [octreeMinBound, octreeMaxBound] = SceneUtilities::calculateOctreeBounds(currentIndexBound, scene);
								auto addedOctree = scene->m_meshFaceOctreesMap.emplace(currentIndexBound, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>(octreeMinBound, octreeMaxBound)).first;
								//neexistuje octree s tymto indexom
								addedOctree->second.addDataToOctree(std::make_pair(mesh, &(*faceIter)), faceBounds);

							}

							HalfEdgeDS::Face* facePointer = &(*faceIter);
							glm::vec3 octreeIndex = currentIndexBound;
							// Now add the entry to the inner map
							faceOctreesMap[facePointer].push_back(octreeIndex);
						}
					}
				}


			}


			m_addedPlane = mesh;
			notifyObservers();
		}

	}
private:
	ViewPortHolder* m_viewPortHolder;
    AddPlaneCommand* m_command;

    
    Mesh* createPlaneVertexData()
    {
		++m_viewPortHolder->m_currentMeshId;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(10.0f, 50.0f);

		float randomY = dis(gen);


        float planeSize = m_command->getPlaneProperties().m_size;
        int planeSubidivisionLevel = m_command->getPlaneProperties().m_subdivisionLevel;

        std::vector<std::vector<int>> planeIndices;
        std::vector<glm::vec3> planeVertices;

        float squareSize = planeSize / planeSubidivisionLevel;
        

        for (int z = 0; z <= planeSubidivisionLevel; ++z)
        {
            for (int x = 0; x <= planeSubidivisionLevel; ++x)
            {
                //VERTEX
                glm::vec3 vertex{ + (-(planeSize / 2)) + (x * squareSize) , 0.0f,  (-(planeSize / 2)) + (z * squareSize)};
                //0x -> (-(planeSize / 2)) + (x * squareSize);
                //0z -> (-(planeSize / 2)) + (z * squareSize);

                planeVertices.push_back(vertex);

                if (x != planeSubidivisionLevel && z != planeSubidivisionLevel)
                {
                    //INDICES
                    int firstIndex = (z * (planeSubidivisionLevel + 1)) + x;
                    int secondIndex = ((z + 1) * (planeSubidivisionLevel + 1)) + x;
                    int thirdIndex = (z * (planeSubidivisionLevel + 1)) + x + 1;
                    int fifthIndex = ((z + 1) * (planeSubidivisionLevel + 1)) + x + 1;
                    //0 -> (z * (planeSubidivisionLevel+1)) + x
                    //5 -> ((z+1) * (planeSubidivisionLevel+1)) + x
                    //1 -> (z * (planeSubidivisionLevel+1)) + x + 1
                    //6 -> ((z+1) * (planeSubidivisionLevel+1)) + x + 1

                    planeIndices.push_back({ firstIndex, secondIndex, thirdIndex });
                    planeIndices.push_back({ fifthIndex, thirdIndex, secondIndex });
                }
            }
        }
        //

        Mesh* mesh = new Mesh(new Material(m_viewPortHolder->m_viewPortLayer->m_meshShader), planeIndices, planeVertices);

		mesh->m_meshID = std::to_string(m_viewPortHolder->m_currentMeshId);
        return mesh;
    }

	void createPlaneRenderingData(Mesh* mesh)
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
			startPoint.y += 0.005f;
			glm::vec3 endPoint = (*edge.getSecondVertex()).getPosition();
			endPoint.y += 0.005f;


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

public:
	Mesh* m_addedPlane = nullptr;


	/*
	void createPlaneRenderingData(Mesh* mesh)
	{
		//edges
		for (auto& edge : mesh->m_halfEdgeMesh->m_edges)
		{
			glm::vec3 startPoint = (*edge.getFirstVertex()).getPosition();
			startPoint.y += 0.005f;
			glm::vec3 endPoint = (*edge.getSecondVertex()).getPosition();
			endPoint.y += 0.005f;
			mesh->m_meshRenderingData.m_edges.push_back({ startPoint, false });
			mesh->m_meshRenderingData.m_edges.push_back({ endPoint, false });
		}

		//vertices
		for (auto& vertex : mesh->m_halfEdgeMesh->m_vertices)
		{
			mesh->m_meshRenderingData.m_points.push_back({ vertex.getPosition(), false });
		}
	}
    */
};
