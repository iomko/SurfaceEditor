#pragma once

#include "Mesh.h"
#include "OctreeSpatialIndex.h"
#include "Camera.h"
#include "../Core/Window.h"
#include "../Core/Profiler.h"
#include "../UI/SelectionRectangle.h"
#include "../Structures/PrintableMesh.h"

#include <algorithm>
#include <concepts>
#include <functional>
#include <limits>
#include <map>
#include <memory>
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
	{ index.getAllElements() } -> std::same_as<std::vector<Element>>;
};

struct SceneResources
{
	using MeshFacePair = FaceRef;
	using PrintableMeshMap = std::map<Mesh *, PrintableMesh *>;

	PrintableMeshMap printableMeshMap;
};

static_assert(SpatialIndex<OctreeSpatialIndex<Mesh *>, Mesh *>);
static_assert(SpatialIndex<OctreeSpatialIndex<ExtendedFace *>, ExtendedFace *>);

using FaceSpatialIndex = OctreeSpatialIndex<ExtendedFace *>;

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
class SceneTemplate
{
public:
	SceneTemplate(MeshIndex meshIndex, std::function<FaceSpatialIndex()> faceIndexFactory);
	~SceneTemplate();

	SceneTemplate(const SceneTemplate &) = delete;
	SceneTemplate &operator=(const SceneTemplate &) = delete;

	void addMesh(Mesh *mesh);
	void addFace(Mesh *mesh, ExtendedFace *face);
	void removeFace(Mesh *mesh, ExtendedFace *face);
	void updateMesh(Mesh *mesh, bool updateFaces = true);
	void removeMesh(Mesh *mesh);
	void query(const Ray &ray, std::vector<FaceRef> &results);
	void query(const AABBBoundingRegion &bounds, std::vector<FaceRef> &results);
	void query(const Sphere &sphere, std::vector<FaceRef> &results);
	std::vector<Mesh *> getAllMeshes() const;

	SceneResources m_res;

private:
	static void insertFace(FaceSpatialIndex &faceIndex, Mesh *mesh, ExtendedFace *face);
	static void appendFaceRefs(
		Mesh *mesh,
		const std::vector<ExtendedFace *> &faces,
		std::vector<FaceRef> &results);

	MeshIndex m_meshIndex;
	std::function<FaceSpatialIndex()> m_faceIndexFactory;
};

using Scene = SceneTemplate<OctreeSpatialIndex<Mesh *>>;

class SceneUtilities
{
public:
	static std::pair<FaceRef, glm::vec3> retClosestHitData(
		Camera *camera,
		Window *window,
		Scene &scene);
	static std::vector<FaceRef> retDataUnderSelectionRectangle(
		const RectanglePos &selectionRect,
		Camera *camera,
		Window *window,
		Scene &scene);
};

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline SceneTemplate<MeshIndex>::SceneTemplate(
	MeshIndex meshIndex,
	std::function<FaceSpatialIndex()> faceIndexFactory)
	: m_meshIndex(std::move(meshIndex)),
	  m_faceIndexFactory(std::move(faceIndexFactory))
{
	if (!m_faceIndexFactory)
	{
		throw std::invalid_argument("A face spatial-index factory is required.");
	}
}

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline SceneTemplate<MeshIndex>::~SceneTemplate()
{
	for (auto &[mesh, printableMesh] : m_res.printableMeshMap)
	{
		delete printableMesh;
	}
	m_res.printableMeshMap.clear();
}

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline void SceneTemplate<MeshIndex>::addMesh(Mesh *mesh)
{
	SE_PROFILE_SCOPE_N("SceneTemplate::addMesh");
	if (mesh == nullptr)
	{
		throw std::invalid_argument("Cannot add a null mesh to the scene.");
	}
	if (mesh->m_faceIndex)
	{
		return;
	}

	auto faceIndex = std::make_unique<FaceSpatialIndex>(m_faceIndexFactory());
	for (ExtendedFace *face : mesh->m_halfEdgeStructure->m_faces)
	{
		insertFace(*faceIndex, mesh, face);
	}
	m_meshIndex.insert(mesh, mesh->m_meshBounds);
	mesh->m_faceIndex = std::move(faceIndex);
}

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline void SceneTemplate<MeshIndex>::addFace(
	Mesh *mesh,
	ExtendedFace *face)
{
	SE_PROFILE_SCOPE_N("SceneTemplate::addFace");
	if (mesh == nullptr || face == nullptr)
	{
		throw std::invalid_argument("Cannot index a null mesh or face.");
	}

	if (!mesh->m_faceIndex)
	{
		throw std::logic_error("Add the mesh to the scene before adding one of its faces.");
	}

	insertFace(*mesh->m_faceIndex, mesh, face);
}

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline void SceneTemplate<MeshIndex>::removeFace(
	Mesh *mesh,
	ExtendedFace *face)
{
	SE_PROFILE_SCOPE_N("SceneTemplate::removeFace");
	if (mesh != nullptr && mesh->m_faceIndex)
	{
		mesh->m_faceIndex->remove(face);
	}
}

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline void SceneTemplate<MeshIndex>::updateMesh(
	Mesh *mesh,
	bool updateFaces)
{
	SE_PROFILE_SCOPE_N("SceneTemplate::updateMesh");
	if (mesh == nullptr || !mesh->m_faceIndex)
	{
		throw std::logic_error("Cannot update a mesh that is not in the scene.");
	}

	m_meshIndex.remove(mesh);
	mesh->calculateMeshBounds();
	m_meshIndex.insert(mesh, mesh->m_meshBounds);

	if (updateFaces == false)
	{
		return;
	}

	auto updatedFaceIndex = std::make_unique<FaceSpatialIndex>(m_faceIndexFactory());
	for (ExtendedFace *face : mesh->m_halfEdgeStructure->m_faces)
	{
		insertFace(*updatedFaceIndex, mesh, face);
	}
	mesh->m_faceIndex = std::move(updatedFaceIndex);
}

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline void SceneTemplate<MeshIndex>::removeMesh(Mesh *mesh)
{
	SE_PROFILE_SCOPE_N("SceneTemplate::removeMesh");
	if (mesh == nullptr || !mesh->m_faceIndex)
	{
		return;
	}

	m_meshIndex.remove(mesh);
	mesh->m_faceIndex.reset();
}

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline void SceneTemplate<MeshIndex>::query(
	const Ray &ray,
	std::vector<FaceRef> &results)
{
	SE_PROFILE_SCOPE_N("SceneTemplate::query");
	std::vector<Mesh *> candidateMeshes;
	m_meshIndex.query(ray, candidateMeshes);

	for (Mesh *mesh : candidateMeshes)
	{
		if (!mesh->m_faceIndex)
		{
			continue;
		}

		std::vector<ExtendedFace *> candidateFaces;
		mesh->m_faceIndex->query(ray, candidateFaces);
		appendFaceRefs(mesh, candidateFaces, results);
	}
}

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline void SceneTemplate<MeshIndex>::query(
	const AABBBoundingRegion &bounds,
	std::vector<FaceRef> &results)
{
	SE_PROFILE_SCOPE_N("SceneTemplate::query");
	std::vector<Mesh *> candidateMeshes;
	m_meshIndex.query(bounds, candidateMeshes);

	for (Mesh *mesh : candidateMeshes)
	{
		if (!mesh->m_faceIndex)
		{
			continue;
		}

		std::vector<ExtendedFace *> candidateFaces;
		mesh->m_faceIndex->query(bounds, candidateFaces);
		appendFaceRefs(mesh, candidateFaces, results);
	}
}

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline void SceneTemplate<MeshIndex>::query(
	const Sphere &sphere,
	std::vector<FaceRef> &results)
{
	SE_PROFILE_SCOPE_N("SceneTemplate::query");
	std::vector<Mesh *> candidateMeshes;
	m_meshIndex.query(sphere, candidateMeshes);

	for (Mesh *mesh : candidateMeshes)
	{
		if (!mesh->m_faceIndex)
		{
			continue;
		}

		std::vector<ExtendedFace *> candidateFaces;
		mesh->m_faceIndex->query(sphere, candidateFaces);
		appendFaceRefs(mesh, candidateFaces, results);
	}
}

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline std::vector<Mesh *> SceneTemplate<MeshIndex>::getAllMeshes() const
{
	SE_PROFILE_SCOPE_N("SceneTemplate::getAllMeshes");
	return m_meshIndex.getAllElements();
}

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline void SceneTemplate<MeshIndex>::insertFace(
	FaceSpatialIndex &faceIndex,
	Mesh *mesh,
	ExtendedFace *face)
{
	SE_PROFILE_SCOPE_N("SceneTemplate::insertFace");
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

template <typename MeshIndex>
	requires SpatialIndex<MeshIndex, Mesh *>
inline void SceneTemplate<MeshIndex>::appendFaceRefs(
	Mesh *mesh,
	const std::vector<ExtendedFace *> &faces,
	std::vector<FaceRef> &results)
{
	SE_PROFILE_SCOPE_N("SceneTemplate::appendFaceRefs");
	for (ExtendedFace *face : faces)
	{
		const FaceRef faceRef{mesh, face};
		results.push_back(faceRef);
	}
}

inline std::pair<FaceRef, glm::vec3> SceneUtilities::retClosestHitData(
	Camera *camera,
	Window *window,
	Scene &scene)
{
	SE_PROFILE_SCOPE_N("SceneUtilities::retClosestHitData");
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

inline std::vector<FaceRef> SceneUtilities::retDataUnderSelectionRectangle(
	const RectanglePos &selectionRect,
	Camera *camera,
	Window *window,
	Scene &scene)
{
	SE_PROFILE_SCOPE_N("SceneUtilities::retDataUnderSelectionRectangle");
	if (camera == nullptr || window == nullptr)
	{
		throw std::invalid_argument("Camera and window are required for rectangle selection.");
	}

	std::vector<FaceRef> selectedFaces;
	const int sampleX = 5;
	const int sampleY = 5;

	for (int i = 0; i < sampleX; ++i)
	{
		const double mouseX = selectionRect.startPos.x +
							  i * (selectionRect.endPos.x - selectionRect.startPos.x) / (sampleX - 1);
		for (int j = 0; j < sampleY; ++j)
		{
			const double mouseY = selectionRect.startPos.y +
								  j * (selectionRect.endPos.y - selectionRect.startPos.y) / (sampleY - 1);
			const Ray ray = Ray::fromPos(*camera, *window, mouseX, mouseY);

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
