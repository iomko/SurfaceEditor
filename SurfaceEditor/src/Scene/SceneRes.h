#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <map>

#include "../Renderer/VertexDataStructs.h"
#include "../AABBBoundingRegion.h"
#include "../Mesh.h"
#include "../DataStructures/Octree.h"

class SceneRes
{
public:
	using MeshLinesVaoMap = std::map<Mesh*, std::vector<LineVertex>>;

	using MaterialVaoMap = std::map<Material*, std::vector<MeshVertex>>;
	using MeshFacesVaoMap = std::map<Mesh*, MaterialVaoMap>;

	using MaterialFacesMap = std::map<Material*, std::vector<HalfEdgeDS::Face*>>;
	using MeshFacesMap = std::map<Mesh*, MaterialFacesMap>;

	struct MeshData
	{
		//pre Mesh VAO
		MeshLinesVaoMap meshLinesVaoMap;
		MeshFacesVaoMap meshFacesVaoMap;
		//pre Mesh Faces
		MeshFacesMap meshFacesMap;
	};

	struct AABBData {
		std::map<AABBBoundingRegion, std::vector<AABBVertex>> vaoDataMap;


		//t·to trieda SceneRes by skÙr mala sl˙ûiù ako
		//len tak˝ holder, Ëiûe metÛdy ako clearAABBData a collectAABBData
		//by som presunul niekde inde.
		void clearAABBData()
		{
			vaoDataMap.clear();
		}

		void collectAABBData(const AABBBoundingRegion& aabb) {
			glm::vec3 vertexColor = glm::vec3(0.0f, 1.0f, 0.0f);
			//left side
			glm::vec3 bottomUpLeft = aabb.getMinBoundsPos();
			glm::vec3 bottomDownLeft = glm::vec3(aabb.getMinBoundsPos().x, aabb.getMinBoundsPos().y, aabb.getMaxBoundsPos().z);
			glm::vec3 upperDownLeft = glm::vec3(aabb.getMinBoundsPos().x, aabb.getMaxBoundsPos().y, aabb.getMaxBoundsPos().z);
			glm::vec3 upperUpLeft = glm::vec3(aabb.getMinBoundsPos().x, aabb.getMaxBoundsPos().y, aabb.getMinBoundsPos().z);
			//right side
			glm::vec3 bottomUpRight = glm::vec3(aabb.getMaxBoundsPos().x, aabb.getMinBoundsPos().y, aabb.getMinBoundsPos().z);
			glm::vec3 bottomDownRight = glm::vec3(aabb.getMaxBoundsPos().x, aabb.getMinBoundsPos().y, aabb.getMaxBoundsPos().z);
			glm::vec3 upperDownRight = aabb.getMaxBoundsPos();
			glm::vec3 upperUpRight = glm::vec3(aabb.getMaxBoundsPos().x, aabb.getMaxBoundsPos().y, aabb.getMinBoundsPos().z);

			AABBVertex AABB_vertices[] = {
				//
				{bottomUpLeft, vertexColor},
				{bottomDownLeft, vertexColor},
				{upperDownLeft, vertexColor},
				{upperDownLeft, vertexColor},
				{upperUpLeft, vertexColor},
				{bottomUpLeft, vertexColor},

				//

				{upperUpLeft, vertexColor},
				{upperDownLeft, vertexColor},
				{upperDownRight, vertexColor},
				{upperDownRight, vertexColor},
				{upperUpRight, vertexColor},
				{upperUpLeft, vertexColor},

				//
				{upperUpRight, vertexColor},
				{upperDownRight, vertexColor},
				{bottomDownRight, vertexColor},
				{bottomDownRight, vertexColor},
				{bottomUpRight, vertexColor},
				{upperUpRight, vertexColor},

				//
				{bottomUpRight, vertexColor},
				{bottomDownRight, vertexColor},
				{bottomDownLeft, vertexColor},
				{bottomDownLeft, vertexColor},
				{bottomUpLeft, vertexColor},
				{bottomUpRight, vertexColor},

				//
				{upperUpLeft, vertexColor},
				{upperUpRight, vertexColor},
				{bottomUpRight, vertexColor},
				{bottomUpRight, vertexColor},
				{bottomUpLeft, vertexColor},
				{upperUpLeft, vertexColor},

				//
				{upperDownLeft, vertexColor},
				{upperDownRight, vertexColor},
				{bottomDownRight, vertexColor},
				{bottomDownRight, vertexColor},
				{bottomDownLeft, vertexColor},
				{upperDownLeft, vertexColor}

			};


			vaoDataMap[aabb].insert(
				vaoDataMap[aabb].end(),
				std::begin(AABB_vertices),
				std::end(AABB_vertices)
			);
		}
	};

	AABBData aabbData;
	MeshData meshData;

	using FaceOctreeCoordsMap = std::map<HalfEdgeDS::Face*, std::vector<glm::vec3>>;
	using MeshFaceOctreeCoordsMap = std::map<Mesh*, FaceOctreeCoordsMap>;
	using MeshFacePair = std::pair<Mesh*, HalfEdgeDS::Face*>;
	using CoordsOctreeMap = std::map<glm::vec3, Octree<MeshFacePair>>;

	MeshFaceOctreeCoordsMap meshFaceOctreeCoordsMap;
	CoordsOctreeMap coordsOctreeMap;
};