#pragma once
#include "../ViewPortsController.h"
#include "../Utils/InterpolationUtils.h"
#include "../Utils/GeometryUtils.h"

class BrushToolCallBack : public Callback<BrushToolParams, OctreeNodeDataParams>, public Observer
{
public:
	void execute(const BrushToolParams& iParams, OctreeNodeDataParams& oParams) override
	{
		const float epsilon = 0.001f;
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
			RendererStageData::MatVertsMap& materialVertsMap = Renderer::s_stageData.meshMatsMap.find(closestMesh)->second;
			std::vector<RendererStageData::LineVertex>& edgesVector = Renderer::s_stageData.meshLinesMap.find(closestMesh)->second;
			
			Sphere sphere{ hitPoint, iParams.radius };
			ExtendedVertex* closestVertex = findClosestVertexOnFace(closestFace, hitPoint);
			glm::vec3 normal = computeAvgNormal(closestVertex);
			auto elementsToChange = collectIntersectingElements(sphere, closestMesh);
			std::unordered_set<ExtendedVertex*>& verticesToChange = elementsToChange.first;
			std::unordered_set<ExtendedFace*>& facesToChange = elementsToChange.second;

			for (ExtendedVertex* vertex : verticesToChange)
			{
				float distance = glm::length(vertex->m_position - sphere.position);
				float scalingFactor = calculateNormalScaleFactor(distance, iParams.radius, iParams.brushStrength);

				for (GraphEdge* graphEdge : vertex->m_graphEdges)
				{
					ExtendedFace* face = graphEdge->face;

					FaceTriangleIndex faceTriangleIndex = face->faceTriangleIndices.front();

					FaceTriangle& faceTriangle =
						closestMesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second.at(faceTriangleIndex);

					std::vector<RendererStageData::MeshVertex>& facesVaoData = materialVertsMap.find(face->material)->second;

					int faceIndexInVao = faceTriangle.indexInVAO;
					for (int i = faceIndexInVao; i < faceIndexInVao + 3; ++i)
					{
						if (glm::all(glm::epsilonEqual(facesVaoData.at(i).position, vertex->m_position, epsilon)))
						{
							facesVaoData.at(i).position += (normal * scalingFactor);
							break;
						}
					}
				}


				for (ExtendedEdge* edge : vertex->m_neighbourEdges)
				{
					int edgeIndexInVao = edge->m_EdgeLineIndex;

					if (glm::all(glm::epsilonEqual(edge->m_firstVertex->m_position, vertex->m_position, epsilon)))
					{
						edgesVector.at(edgeIndexInVao).position += (normal * scalingFactor);
						edgesVector.at(edgeIndexInVao + 2).position += (normal * scalingFactor);
					}
					else if (glm::all(glm::epsilonEqual(edge->m_secondVertex->m_position, vertex->m_position, epsilon)))
					{
						edgesVector.at(edgeIndexInVao + 1).position += (normal * scalingFactor);
						edgesVector.at(edgeIndexInVao + 3).position += (normal * scalingFactor);
					}
				}

				vertex->m_position += (normal * scalingFactor);
			}


			for (ExtendedFace* face : facesToChange)
			{
				scene->deleteFaceFromOctrees(closestMesh, face);
				scene->addFaceIntoOctrees(closestMesh, face);
			}
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
			vertex->m_position;
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
