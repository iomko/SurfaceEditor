#pragma once
#include "../ViewPortsController.h"
#include "../Utils/InterpolationUtils.h"
#include "../Utils/GeometryUtils.h"
#include "MoveVertexCallBack.h"

class BrushToolCallBack : public Callback<BrushToolParams, OctreeNodeDataParams>, public Observer
{
public:

	void execute(const BrushToolParams& iParams, OctreeNodeDataParams& oParams) override
	{
        
		const float epsilon = 0.001f;

        //Get closest mesh and it's face that was hit by the ray from the camera
		Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
		Camera* camera = ViewPortsHolderContext::s_camera;
		Window* window = ViewPortsHolderContext::s_window;
		std::pair<SceneResources::MeshFacePair, glm::vec3> meshFaceHitPair = SceneUtilities::retClosestHitData(camera, window, scene->m_res);
		SceneResources::MeshFacePair meshFacePair = meshFaceHitPair.first;

		glm::vec3 hitPoint = meshFaceHitPair.second;
		Mesh* closestMesh = meshFacePair.first;
		ExtendedFace* closestFace = meshFacePair.second;

		oParams.hitPoint = hitPoint;
		oParams.meshFacePair = meshFacePair;

		if (closestMesh != nullptr)
		{
            //Find vertex on the face, which was closest to the hit point
			Sphere sphere{ hitPoint, iParams.radius };
			ExtendedVertex* closestVertex = findClosestVertexOnFace(closestFace, hitPoint);
            
            //Collect all data that needs to change/move
			glm::vec3 normal = computeAvgNormal(closestVertex);
			auto elementsToChange = collectIntersectingElements(sphere, closestMesh);
			std::unordered_set<ExtendedVertex*>& verticesToChange = elementsToChange.first;
			//std::unordered_set<ExtendedFace*>& facesToChange = elementsToChange.second;

			for (ExtendedVertex* vertex : verticesToChange)
			{
				float distance = glm::length(vertex->m_position - sphere.position);
				float scalingFactor = calculateNormalScaleFactor(distance, iParams.radius, iParams.brushStrength);

                //moveByVector
                glm::vec3 moveByVector = normal * scalingFactor;

                MoveVertexCallBack moveVertexCallBack;
                VertexParams vertexParams;
                vertexParams.newPosition = moveByVector;
                vertexParams.mesh = closestMesh;
                vertexParams.vertex = vertex;
                moveVertexCallBack.execute(vertexParams);

			}

            /*
			for (ExtendedFace* face : facesToChange)
			{
				scene->deleteFaceFromOctrees(closestMesh, face);
				scene->addFaceIntoOctrees(closestMesh, face);
			}
            */
		}
    
	}



private:

	float calculateNormalScaleFactor(float distance, float radius, float brushStrength) {
		float t = 1.0f - (distance / radius);
		t = std::clamp(t, 0.0f, 1.0f);

		float scalingFactor = utils::interpolation::smoothstep(0.0f, 1.0f, t);
		scalingFactor *= brushStrength;

		return scalingFactor;
	}

	glm::vec3 computeAvgNormal(ExtendedVertex* vertex)
	{
		glm::vec3 averageNormal(0.0f);

		for (auto* graphEdge : vertex->m_graphEdges)
		{
			glm::vec3 faceNormal = utils::geometry::computePolygonNormal(graphEdge->face);

			averageNormal += faceNormal;
		}

		if (glm::length(averageNormal) > 0.0f)
			averageNormal = glm::normalize(averageNormal);

		return averageNormal;
	}

	std::pair<std::unordered_set<ExtendedVertex*>, std::unordered_set<ExtendedFace*>>
	collectIntersectingElements(const Sphere& sphere, Mesh* mesh)
	{
		Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
		std::unordered_set<ExtendedVertex*> verticesToChange;
		std::unordered_set<ExtendedFace*> facesToChange;

		auto octreeBoundsIntersectAlg = [](const AABBBoundingRegion& aabb, const Sphere& sphereIn)
			{
				return aabb.intersectsSphere(sphereIn);
			};

		auto dataIntersectAlg = [&](const std::pair<Mesh*, ExtendedFace*>& meshFacePair, const Sphere& sphereIn)
			{
				bool intersects = false;
				Mesh* meshIn = meshFacePair.first;
				ExtendedFace* face = meshFacePair.second;
				if (meshIn != mesh)
				{
					return intersects;
				}
				else
				{
					for (auto it = face->faceVertexBegin(); it != face->faceVertexEnd(); ++it)
					{
						ExtendedVertex* vertex = &it.operator*();
						if (sphereIn.containsPoint(vertex->m_position))
						{
							intersects = true;
							verticesToChange.insert(vertex);
						}
					}
				}

				if(intersects)
				{
					facesToChange.insert(face);
				}
				return intersects;
			};

		for (auto& entry : scene->m_res.coordsOctreeMap)
		{
			Octree<SceneResources::MeshFacePair>& octree = entry.second;
			octree.findDataInOctree<Sphere>(sphere, octreeBoundsIntersectAlg, dataIntersectAlg);
		}

		return { verticesToChange, facesToChange };
	}

	ExtendedVertex* findClosestVertexOnFace(ExtendedFace* face, glm::vec3 hitPoint)
	{
		ExtendedVertex* closestVertex = nullptr;
		for (auto it = face->faceVertexBegin(); it != face->faceVertexEnd(); ++it) {
			ExtendedVertex* vertex = &it.operator*();

			if(closestVertex == nullptr)
			{
				closestVertex = vertex;
			} else
			{
				float closestVertexLength = glm::length(closestVertex->m_position - hitPoint);
				float newVertexLength = glm::length(vertex->m_position - hitPoint);
				if(newVertexLength < closestVertexLength)
				{
					closestVertex = vertex;
				}
			}
		}
		return closestVertex;
	}
};
