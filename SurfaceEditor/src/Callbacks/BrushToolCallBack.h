#pragma once
#include "../ViewPortsHolder.h"
#include "../Utils/InterpolationUtils.h"

class BrushToolCallBack : public Callback<BrushToolParams, OctreeNodeDataParams>, public Observer
{
public:
	void execute(const BrushToolParams& iParams, OctreeNodeDataParams& oParams) override
	{
		const float epsilon = 0.001f;
		Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;
		Camera* camera = ViewPortsHolderContext::m_camera;
		Window* window = ViewPortsHolderContext::m_window;
		std::pair<SceneRes::MeshFacePair, glm::vec3> meshFaceHitPair = SceneUtilities::retClosestHitData(camera, window, scene->m_res);
		SceneRes::MeshFacePair meshFacePair = meshFaceHitPair.first;

		glm::vec3 hitPoint = meshFaceHitPair.second;
		Mesh* closestMesh = meshFacePair.first;
		HalfEdgeDS::Face* closestFace = meshFacePair.second;

		oParams.hitPoint = hitPoint;
		oParams.meshFacePair = meshFacePair;

		if (closestMesh != nullptr)
		{
			SceneRes::MaterialVaoMap& materialFacesVaoMap = scene->m_res.meshData.meshFacesVaoMap.find(closestMesh)->second;
			std::vector<LineVertex>& edgesVaoMap = scene->m_res.meshData.meshLinesVaoMap.find(closestMesh)->second;
			
			Sphere sphere{ hitPoint, iParams.radius };
			HalfEdgeDS::Vertex* closestVertex = findNearestVertex(closestFace, hitPoint);
			glm::vec3 normal = calculateNormal(closestVertex);
			std::pair<std::unordered_set<HalfEdgeDS::Vertex*>, std::unordered_set<HalfEdgeDS::Face*>> objectsToChange =
				findIntersections(sphere, closestMesh);
			std::unordered_set<HalfEdgeDS::Vertex*>& verticesToChange = objectsToChange.first;
			std::unordered_set<HalfEdgeDS::Face*>& facesToChange = objectsToChange.second;

			for (HalfEdgeDS::Vertex* vertex : verticesToChange)
			{
				float distance = glm::length(vertex->m_position - sphere.position);
				float scalingFactor = calculateBrushScalingFactor(distance, iParams.radius, iParams.brushStrength);

				for (HalfEdgeDS::GraphEdge* graphEdge : vertex->m_graphEdges)
				{
					HalfEdgeDS::Face* face = graphEdge->face;

					FaceTriangleIndex faceTriangleIndex = face->faceTriangleIndices.front();

					HalfEdgeDS::FaceTriangle& faceTriangle =
						closestMesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second.at(faceTriangleIndex);

					std::vector<MeshVertex>& facesVaoData = materialFacesVaoMap.find(face->material)->second;

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


				for (HalfEdgeDS::Edge* edge : vertex->m_neighbourEdges)
				{
					int edgeIndexInVao = edge->m_EdgeLineIndex;

					if (glm::all(glm::epsilonEqual(edge->m_firstVertex->m_position, vertex->m_position, epsilon)))
					{
						edgesVaoMap.at(edgeIndexInVao).position += (normal * scalingFactor);
						edgesVaoMap.at(edgeIndexInVao + 2).position += (normal * scalingFactor);
					}
					else if (glm::all(glm::epsilonEqual(edge->m_secondVertex->m_position, vertex->m_position, epsilon)))
					{
						edgesVaoMap.at(edgeIndexInVao + 1).position += (normal * scalingFactor);
						edgesVaoMap.at(edgeIndexInVao + 3).position += (normal * scalingFactor);
					}
				}

				vertex->m_position += (normal * scalingFactor);
			}

			updateFacesInOctrees(closestMesh, facesToChange);
		}
	}

private:

	void updateFacesInOctrees(Mesh* mesh, const std::unordered_set<HalfEdgeDS::Face*>& faces)
	{
		Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;

		for (HalfEdgeDS::Face* face : faces)
		{
			scene->deleteFaceFromOctrees(mesh, face);
			scene->addFaceIntoOctrees(mesh, face);
		}
	}

	float calculateBrushScalingFactor(float distance, float radius, float brushStrength) {
		float t = 1.0f - (distance / radius);
		t = std::clamp(t, 0.0f, 1.0f);

		float scalingFactor = utils::interpolation::smoothstep(0.0f, 1.0f, t);
		scalingFactor *= brushStrength;

		return scalingFactor;
	}

	glm::vec3 calculateNormal(HalfEdgeDS::Vertex* vertex)
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

	std::pair<std::unordered_set<HalfEdgeDS::Vertex*>, std::unordered_set<HalfEdgeDS::Face*>>
	findIntersections(const Sphere& sphere, Mesh* hitMesh)
	{
		Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;
		std::unordered_set<HalfEdgeDS::Vertex*> verticesToChange;
		std::unordered_set<HalfEdgeDS::Face*> facesToChange;

		auto octreeBoundsIntersectAlg = [](const AABBBoundingRegion& aabb, const Sphere& sphereIn)
			{
				return aabb.intersectsSphere(sphereIn);
			};

		auto dataIntersectAlg = [&](const std::pair<Mesh*, HalfEdgeDS::Face*>& meshFacePair, const Sphere& sphereIn)
			{
				bool intersects = false;
				Mesh* mesh = meshFacePair.first;
				HalfEdgeDS::Face* face = meshFacePair.second;
				if (mesh != hitMesh)
				{
					return intersects;
				}
				else
				{
					for (auto it = face->faceVertexBegin(); it != face->faceVertexEnd(); ++it)
					{
						HalfEdgeDS::Vertex* vertex = &it.operator*();
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
			Octree<SceneRes::MeshFacePair>& octree = entry.second;
			octree.findDataInOctree<Sphere>(sphere, octreeBoundsIntersectAlg, dataIntersectAlg);
		}

		return { verticesToChange, facesToChange };
	}

	HalfEdgeDS::Vertex* findNearestVertex(HalfEdgeDS::Face* face, glm::vec3 hitPoint)
	{
		HalfEdgeDS::Vertex* closestVertex = nullptr;
		for (auto it = face->faceVertexBegin(); it != face->faceVertexEnd(); ++it) {
			HalfEdgeDS::Vertex* vertex = &it.operator*();
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