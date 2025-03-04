#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <map>

#include "../Renderer/VertexDataStructs.h"
#include "../AABBBoundingRegion.h"
#include "../Mesh.h"

class SceneRendererData
{
public:
	struct FaceInfo
	{
		glm::vec3 normal;
		size_t startIndex;
		size_t endIndex;
		size_t indexInMaterial;
		Material* material;
	};

	using MaterialVertexMap = std::map<Material*, std::vector<MeshVertex>>;
	using MeshVaoDataMap = std::map<Mesh*, MaterialVertexMap>;

	//using FaceInfoMap = std::map<HalfEdgeDS::Face*, FaceInfo>;
	using FaceInfoMap = std::map<HalfEdgeDS::FaceIndex, FaceInfo>;
	using MeshFacesMap = std::map<Mesh*, FaceInfoMap>;

	//using MaterialFacesMap = std::map<Material*, std::vector<HalfEdgeDS::Face*>>;
	using MaterialFacesMap = std::map<Material*, std::vector<HalfEdgeDS::FaceIndex>>;
	using MeshMaterialsMap = std::map<Mesh*, MaterialFacesMap>;

	struct MeshData
	{
		//pre Mesh VAO
		MeshVaoDataMap meshVaoDataMap;

		//pre Mesh
		MeshFacesMap meshFacesMap;
		MeshMaterialsMap meshMaterialsMap;
		//std::map<HalfEdgeDS::Face*, FaceInfo> faceInfoMap;
	};

	struct AABBData {
		std::map<AABBBoundingRegion, std::vector<AABBVertex>> vaoDataMap;


		//t·to trieda SceneRendererData by skÙr mala sl˙ûiù ako
		//len tak˝ holder, Ëiûe metÛdy ako clearAABBData a collectAABBData
		//by som presunul niekde inde.
		void clearAABBData()
		{
			vaoDataMap.clear();
		}

		void collectAABBData(const AABBBoundingRegion& aabb) {
			glm::vec3 vertexColor = glm::vec3(0.0f, 1.0f, 0.0f);
			//left side
			glm::vec3 bottomUpLeft = aabb.getMin();
			glm::vec3 bottomDownLeft = glm::vec3(aabb.getMin().x, aabb.getMin().y, aabb.getMax().z);
			glm::vec3 upperDownLeft = glm::vec3(aabb.getMin().x, aabb.getMax().y, aabb.getMax().z);
			glm::vec3 upperUpLeft = glm::vec3(aabb.getMin().x, aabb.getMax().y, aabb.getMin().z);
			//right side
			glm::vec3 bottomUpRight = glm::vec3(aabb.getMax().x, aabb.getMin().y, aabb.getMin().z);
			glm::vec3 bottomDownRight = glm::vec3(aabb.getMax().x, aabb.getMin().y, aabb.getMax().z);
			glm::vec3 upperDownRight = aabb.getMax();
			glm::vec3 upperUpRight = glm::vec3(aabb.getMax().x, aabb.getMax().y, aabb.getMin().z);

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
};