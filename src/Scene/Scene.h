#pragma once

#include "../Scene/Mesh.h"
#include "../Primitives/AABB.h"
#include "../Structures/Octree.h"
#include "../Scene/Camera.h"
#include "../Core/Window.h"
#include "../UI/SelectionRectangle.h"
#include "Structures/PrintableMesh.h"

#include <algorithm>
#include <concepts>
#include <functional>
#include <limits>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

using FaceRef = std::pair<Mesh *, ExtendedFace *>;

template <typename Index, typename Element>
concept SpatialIndex = requires(
	Index &index,
	const Element &element,
	const AABBBoundingRegion &bounds,
	const Ray &ray,
	const Sphere &sphere,
	std::vector<Element> &results) {
	{ index.insert(element, bounds) } -> std::same_as<void>;
	{ index.remove(element) } -> std::same_as<void>;
	{ index.query(ray, results) } -> std::same_as<void>;
	{ index.query(sphere, results) } -> std::same_as<void>;
	{ index.query(bounds, results) } -> std::same_as<void>;
};

struct SceneResources
{
	using MeshFacePair = FaceRef;
	using PrintableMeshMap = std::map<Mesh *, PrintableMesh *>;

	PrintableMeshMap printableMeshMap;
};

template <typename Element>
class OctreeSpatialIndex
{
public:
	explicit OctreeSpatialIndex(const glm::vec3 &voxelSize)
		: m_voxelSize(voxelSize)
	{
		if (voxelSize.x <= 0.0f || voxelSize.y <= 0.0f || voxelSize.z <= 0.0f)
		{
			throw std::invalid_argument("Spatial-index voxel sizes must be positive.");
		}
	}

	void insert(const Element &element, const AABBBoundingRegion &bounds)
	{
		// remove(element); expected to be called before insert if the element already exists

		const glm::vec3 minCell = getVoxelIndex(bounds.getMinBoundsPos());
		const glm::vec3 maxCell = getVoxelIndex(bounds.getMaxBoundsPos());
		auto &elementCells = m_elementCells[element];

		for (int x = static_cast<int>(minCell.x); x <= static_cast<int>(maxCell.x); ++x)
		{
			for (int y = static_cast<int>(minCell.y); y <= static_cast<int>(maxCell.y); ++y)
			{
				for (int z = static_cast<int>(minCell.z); z <= static_cast<int>(maxCell.z); ++z)
				{
					const glm::vec3 cell{
						static_cast<float>(x),
						static_cast<float>(y),
						static_cast<float>(z)};
					auto treeIt = m_trees.find(cell);
					if (treeIt == m_trees.end())
					{
						const glm::vec3 minBounds = cell * m_voxelSize;
						const glm::vec3 maxBounds = minBounds + m_voxelSize;
						treeIt = m_trees.emplace(
											cell,
											Octree<Element>(minBounds, maxBounds))
									 .first;
					}

					treeIt->second.addDataToOctree(element, bounds);
					elementCells.push_back(cell);
				}
			}
		}
	}

	void remove(const Element &element)
	{
		const auto elementIt = m_elementCells.find(element);
		if (elementIt == m_elementCells.end())
		{
			return;
		}

		for (const glm::vec3 &cell : elementIt->second)
		{
			const auto treeIt = m_trees.find(cell);
			if (treeIt == m_trees.end())
			{
				continue;
			}

			treeIt->second.removeData(element);
			if (treeIt->second.isEmpty())
			{
				m_trees.erase(treeIt);
			}
		}

		m_elementCells.erase(elementIt);
	}

	void query(const Ray &ray, std::vector<Element> &results)
	{
		for (auto &[cell, tree] : m_trees)
		{
			appendQueryResults(
				tree,
				ray,
				[](const AABBBoundingRegion &nodeBounds, const Ray &queryRay)
				{
					return nodeBounds.intersectsRay(queryRay);
				},
				results);
		}
	}

	void query(const AABBBoundingRegion &bounds, std::vector<Element> &results)
	{
		for (auto &[cell, tree] : m_trees)
		{
			appendQueryResults(
				tree,
				bounds,
				[](const AABBBoundingRegion &nodeBounds, const AABBBoundingRegion &queryBounds)
				{
					return nodeBounds.intersectsAABB(queryBounds);
				},
				results);
		}
	}

	void query(const Sphere &sphere, std::vector<Element> &results)
	{
		for (auto &[cell, tree] : m_trees)
		{
			appendQueryResults(
				tree,
				sphere,
				[](const AABBBoundingRegion &nodeBounds, const Sphere &querySphere)
				{
					return nodeBounds.intersectsSphere(querySphere);
				},
				results);
		}
	}

private:
	template <typename QueryShape, typename Intersect>
	static void appendQueryResults(
		Octree<Element> &tree,
		const QueryShape &queryShape,
		Intersect intersects,
		std::vector<Element> &results)
	{
		std::vector<OctreeNode<Element> *> nodes;
		tree.template findMaxDepthNodes<QueryShape>(queryShape, intersects, nodes);
		for (const OctreeNode<Element> *node : nodes)
		{
			for (const Element &element : node->nodeData)
			{
				if (std::find(results.begin(), results.end(), element) == results.end())
				{
					results.push_back(element);
				}
			}
		}
	}

	glm::vec3 getVoxelIndex(const glm::vec3 &position) const
	{
		return glm::floor(position / m_voxelSize);
	}

	glm::vec3 m_voxelSize;
	std::map<glm::vec3, Octree<Element>> m_trees;
	std::map<Element, std::vector<glm::vec3>> m_elementCells;
};

static_assert(SpatialIndex<OctreeSpatialIndex<Mesh *>, Mesh *>);
static_assert(SpatialIndex<OctreeSpatialIndex<ExtendedFace *>, ExtendedFace *>);

template <typename MeshIndex, typename FaceIndex>
	requires SpatialIndex<MeshIndex, Mesh *> && SpatialIndex<FaceIndex, ExtendedFace *>
class SceneTemplate
{
public:
	SceneTemplate(MeshIndex meshIndex, std::function<FaceIndex()> faceIndexFactory)
		: m_meshIndex(std::move(meshIndex)),
		  m_faceIndexFactory(std::move(faceIndexFactory))
	{
		if (!m_faceIndexFactory)
		{
			throw std::invalid_argument("A face spatial-index factory is required.");
		}
	}

	~SceneTemplate()
	{
		for (auto &[mesh, printableMesh] : m_res.printableMeshMap)
		{
			delete printableMesh;
		}
		m_res.printableMeshMap.clear();
	}

	SceneTemplate(const SceneTemplate &) = delete;
	SceneTemplate &operator=(const SceneTemplate &) = delete;

	void addMesh(Mesh *mesh)
	{
		if (mesh == nullptr)
		{
			throw std::invalid_argument("Cannot add a null mesh to the scene.");
		}
		if (m_faceIndices.contains(mesh))
		{
			return;
		}

		m_meshIndex.insert(mesh, mesh->m_meshBounds);
		auto [faceIndexIt, inserted] = m_faceIndices.try_emplace(mesh, m_faceIndexFactory());
		(void)inserted;

		for (ExtendedFace *face : mesh->m_halfEdgeStructure->m_faces)
		{
			insertFace(faceIndexIt->second, mesh, face);
		}
	}

	void addFace(Mesh *mesh, ExtendedFace *face)
	{
		if (mesh == nullptr || face == nullptr)
		{
			throw std::invalid_argument("Cannot index a null mesh or face.");
		}

		const auto faceIndexIt = m_faceIndices.find(mesh);
		if (faceIndexIt == m_faceIndices.end())
		{
			throw std::logic_error("Add the mesh to the scene before adding one of its faces.");
		}

		insertFace(faceIndexIt->second, mesh, face);
	}

	void removeFace(Mesh *mesh, ExtendedFace *face)
	{
		const auto faceIndexIt = m_faceIndices.find(mesh);
		if (faceIndexIt != m_faceIndices.end())
		{
			faceIndexIt->second.remove(face);
		}
	}

	void updateMesh(Mesh *mesh)
	{
		const auto faceIndexIt = m_faceIndices.find(mesh);
		if (faceIndexIt == m_faceIndices.end())
		{
			throw std::logic_error("Cannot update a mesh that is not in the scene.");
		}

		m_meshIndex.remove(mesh);
		mesh->calculateMeshBounds();
		m_meshIndex.insert(mesh, mesh->m_meshBounds);

		m_faceIndices.erase(faceIndexIt);
		auto [updatedFaceIndexIt, inserted] = m_faceIndices.try_emplace(mesh, m_faceIndexFactory());
		(void)inserted;
		for (ExtendedFace *face : mesh->m_halfEdgeStructure->m_faces)
		{
			insertFace(updatedFaceIndexIt->second, mesh, face);
		}
	}

	void removeMesh(Mesh *mesh)
	{
		const auto faceIndexIt = m_faceIndices.find(mesh);
		if (faceIndexIt == m_faceIndices.end())
		{
			return;
		}

		m_meshIndex.remove(mesh);
		m_faceIndices.erase(faceIndexIt);
	}

	void query(const Ray &ray, std::vector<FaceRef> &results)
	{
		std::vector<Mesh *> candidateMeshes;
		m_meshIndex.query(ray, candidateMeshes);

		for (Mesh *mesh : candidateMeshes)
		{
			const auto faceIndexIt = m_faceIndices.find(mesh);
			if (faceIndexIt == m_faceIndices.end())
			{
				continue;
			}

			std::vector<ExtendedFace *> candidateFaces;
			faceIndexIt->second.query(ray, candidateFaces);
			appendFaceRefs(mesh, candidateFaces, results);
		}
	}

	void query(const AABBBoundingRegion &bounds, std::vector<FaceRef> &results)
	{
		std::vector<Mesh *> candidateMeshes;
		m_meshIndex.query(bounds, candidateMeshes);

		for (Mesh *mesh : candidateMeshes)
		{
			const auto faceIndexIt = m_faceIndices.find(mesh);
			if (faceIndexIt == m_faceIndices.end())
			{
				continue;
			}

			std::vector<ExtendedFace *> candidateFaces;
			faceIndexIt->second.query(bounds, candidateFaces);
			appendFaceRefs(mesh, candidateFaces, results);
		}
	}

	void query(const Sphere &sphere, std::vector<FaceRef> &results)
	{
		std::vector<Mesh *> candidateMeshes;
		m_meshIndex.query(sphere, candidateMeshes);

		for (Mesh *mesh : candidateMeshes)
		{
			const auto faceIndexIt = m_faceIndices.find(mesh);
			if (faceIndexIt == m_faceIndices.end())
			{
				continue;
			}

			std::vector<ExtendedFace *> candidateFaces;
			faceIndexIt->second.query(sphere, candidateFaces);
			appendFaceRefs(mesh, candidateFaces, results);
		}
	}

	std::vector<Mesh *> getAllMeshes() const
	{
		std::vector<Mesh *> meshes;
		for (const auto &[mesh, _] : m_faceIndices)
		{
			meshes.push_back(mesh);
		}
		return meshes;
	}

	SceneResources m_res;

private:
	static void insertFace(FaceIndex &faceIndex, Mesh *mesh, ExtendedFace *face)
	{
		std::vector<ExtendedVertex> vertices;
		mesh->m_halfEdgeStructure->getVerticesFromFace(face, vertices);
		if (vertices.empty())
		{
			return;
		}

		const AABBBoundingRegion bounds(
			vertices.begin(),
			vertices.end(),
			[](const ExtendedVertex &vertex)
			{
				return vertex.m_position;
			});
		faceIndex.insert(face, bounds);
	}

	static void appendFaceRefs(
		Mesh *mesh,
		const std::vector<ExtendedFace *> &faces,
		std::vector<FaceRef> &results)
	{
		for (ExtendedFace *face : faces)
		{
			const FaceRef faceRef{mesh, face};
			if (std::find(results.begin(), results.end(), faceRef) == results.end())
			{
				results.push_back(faceRef);
			}
		}
	}

	MeshIndex m_meshIndex;
	std::function<FaceIndex()> m_faceIndexFactory;
	std::map<Mesh *, FaceIndex> m_faceIndices;
};

using Scene = SceneTemplate<
	OctreeSpatialIndex<Mesh *>,
	OctreeSpatialIndex<ExtendedFace *>>;

class SceneUtilities
{
public:
	static std::pair<FaceRef, glm::vec3> retClosestHitData(Camera *camera, Window *window, Scene &scene)
	{
		if (camera == nullptr || window == nullptr)
		{
			throw std::invalid_argument("Camera and window are required for scene picking.");
		}

		const Ray ray = Ray::fromMousePos(*camera, *window);
		std::vector<FaceRef> candidates;
		scene.query(ray, candidates);

		FaceRef closestFace{nullptr, nullptr};
		float closestDistance = std::numeric_limits<float>::max();

		for (const FaceRef &candidate : candidates)
		{
			Mesh *mesh = candidate.first;
			ExtendedFace *face = candidate.second;
			if (mesh == nullptr || face == nullptr)
			{
				continue;
			}

			const auto triangleStorage = mesh->bufferLayout.getTriangleBufferStorage(face->material);
			const auto trianglesIt = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material);
			if (!triangleStorage || trianglesIt == mesh->m_halfEdgeStructure->m_faceTriangles.end())
			{
				continue;
			}

			const auto &vertices = triangleStorage->get().data.vertices;
			const auto &faceTriangles = trianglesIt->second;
			for (FaceTriangleIndex triangleIndex : face->faceTriangleIndices)
			{
				if (triangleIndex < 0 || static_cast<size_t>(triangleIndex) >= faceTriangles.size())
				{
					continue;
				}

				const int vertexStart = faceTriangles[triangleIndex].indexInVAO;
				if (vertexStart < 0 ||
					static_cast<size_t>(vertexStart) + 2 >= vertices.size())
				{
					continue;
				}

				const glm::vec3 &firstVertex = vertices[vertexStart].position;
				const glm::vec3 &secondVertex = vertices[vertexStart + 1].position;
				const glm::vec3 &thirdVertex = vertices[vertexStart + 2].position;

				float distance = 0.0f;
				if (Ray::intersectsTriangle(
						ray.origin,
						ray.direction,
						firstVertex,
						secondVertex,
						thirdVertex,
						distance) &&
					distance < closestDistance)
				{
					closestDistance = distance;
					closestFace = candidate;
				}
			}
		}

		if (closestFace.first == nullptr)
		{
			return {closestFace, glm::vec3(0.0f)};
		}

		return {closestFace, ray.origin + ray.direction * closestDistance};
	}

	static std::vector<FaceRef> retDataUnderSelectionRectangle(
		const RectanglePos &selectionRect,
		Camera *camera,
		Window *window,
		Scene &scene)
	{
		if (camera == nullptr || window == nullptr)
		{
			throw std::invalid_argument("Camera and window are required for rectangle selection.");
		}

		std::vector<FaceRef> selectedFaces;

		const int sampleX = 5;
		const int sampleY = 5;

		for (int i = 0; i < sampleX; ++i)
		{
			double mouseX = selectionRect.startPos.x + i * (selectionRect.endPos.x - selectionRect.startPos.x) / (sampleX - 1);
			for (int j = 0; j < sampleY; ++j)
			{
				double mouseY = selectionRect.startPos.y + j * (selectionRect.endPos.y - selectionRect.startPos.y) / (sampleY - 1);

				Ray ray = Ray::fromPos(*camera, *window, mouseX, mouseY);

				std::vector<FaceRef> candidates;
				scene.query(ray, candidates);

				for (const FaceRef &candidate : candidates)
				{
					Mesh *mesh = candidate.first;
					ExtendedFace *face = candidate.second;
					if (mesh == nullptr || face == nullptr)
					{
						continue;
					}

					const auto triangleStorage = mesh->bufferLayout.getTriangleBufferStorage(face->material);
					const auto trianglesIt = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material);
					if (!triangleStorage || trianglesIt == mesh->m_halfEdgeStructure->m_faceTriangles.end())
					{
						continue;
					}

					const auto &vertices = triangleStorage->get().data.vertices;
					const auto &faceTriangles = trianglesIt->second;
					bool intersectsFace = false;
					for (FaceTriangleIndex triangleIndex : face->faceTriangleIndices)
					{
						if (triangleIndex < 0 ||
							static_cast<size_t>(triangleIndex) >= faceTriangles.size())
						{
							continue;
						}

						const int vertexStart = faceTriangles[triangleIndex].indexInVAO;
						if (vertexStart < 0 ||
							static_cast<size_t>(vertexStart) + 2 >= vertices.size())
						{
							continue;
						}

						float distance = 0.0f;
						if (Ray::intersectsTriangle(
								ray.origin,
								ray.direction,
								vertices[vertexStart].position,
								vertices[vertexStart + 1].position,
								vertices[vertexStart + 2].position,
								distance))
						{
							intersectsFace = true;
							break;
						}
					}

					if (intersectsFace &&
						std::find(selectedFaces.begin(), selectedFaces.end(), candidate) == selectedFaces.end())
					{
						selectedFaces.push_back(candidate);
					}
				}
			}
		}

		return selectedFaces;
	}
};