#include "../src/Core/Application.h"
#include "../src/Scene/Scene.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace
{
std::vector<std::vector<int>> trianglePolygons{{0, 1, 2}};
const std::vector<glm::vec3> triangleVertices{
	{-1.0f, -1.0f, 0.0f},
	{1.0f, -1.0f, 0.0f},
	{0.0f, 1.0f, 0.0f}};

Scene makeScene()
{
	const glm::vec3 cellSize{10.0f};
	return Scene{cellSize};
}

std::vector<FaceRef> queryRay(Scene &scene, const glm::vec3 &origin)
{
	std::vector<FaceRef> results;
	scene.query(Ray{origin, {0.0f, 0.0f, 1.0f}}, results);
	return results;
}

void populateRenderableFace(
	Mesh &mesh,
	Material &material,
	glm::vec3 offset,
	const std::vector<glm::vec3> &vertices = triangleVertices)
{
	ExtendedFace *face = mesh.m_halfEdgeStructure->m_faces.front();
	face->material = &material;
	face->faceTriangleIndices = {0};
	mesh.m_halfEdgeStructure->m_faceTriangles[&material] = {{0, face, 0}};

	auto &storage = mesh.bufferLayout.registerTriangleBufferStorage(&material);
	for (glm::vec3 position : vertices)
	{
		position += offset;
		BufferStorageDataType::TriangleVertex vertex;
		vertex.position = position;
		storage.data.vertices.push_back(vertex);
	}
}

std::vector<glm::vec3> triangleAtScreenPositions(
	Window &window,
	const std::array<glm::vec2, 3> &screenPositions)
{
	const float depth = 2.0f;
	const float halfFovRadians = glm::radians(45.0f) * 0.5f;
	std::vector<glm::vec3> vertices;
	vertices.reserve(screenPositions.size());
	for (const glm::vec2 &screenPosition : screenPositions)
	{
		const float normalizedX =
			2.0f * screenPosition.x / window.getScreenWidth() - 1.0f;
		const float normalizedY =
			1.0f - 2.0f * screenPosition.y / window.getScreenHeight();
		vertices.emplace_back(
			normalizedX * depth * std::tan(halfFovRadians) *
				static_cast<float>(window.getScreenWidth()) / window.getScreenHeight(),
			normalizedY * depth * std::tan(halfFovRadians),
			-1.0f);
	}
	return vertices;
}

Camera makePickingCamera(Window &window, const glm::vec3 &rayDirection)
{
	Camera camera{{0.0f, 0.0f, -3.0f}};
	camera.m_matrices.perspectiveMatrix = glm::perspective(
		glm::radians(45.0f),
		static_cast<float>(window.getScreenWidth()) / window.getScreenHeight(),
		0.1f,
		100.0f);

	double mouseX = 0.0;
	double mouseY = 0.0;
	glfwGetCursorPos(window.getWindowHandle(), &mouseX, &mouseY);
	const glm::vec4 clipPosition{
		(2.0f * static_cast<float>(mouseX)) / window.getScreenWidth() - 1.0f,
		1.0f - (2.0f * static_cast<float>(mouseY)) / window.getScreenHeight(),
		-1.0f,
		1.0f};
	glm::vec4 cameraSpaceRay =
		glm::inverse(camera.m_matrices.perspectiveMatrix) * clipPosition;
	cameraSpaceRay.z = -1.0f;
	cameraSpaceRay.w = 0.0f;

	const glm::quat cameraRotation = glm::rotation(
		glm::normalize(glm::vec3{cameraSpaceRay}),
		glm::normalize(rayDirection));
	const glm::mat4 cameraTransform =
		glm::translate(glm::mat4{1.0f}, camera.getState().position) *
		glm::mat4_cast(cameraRotation);
	camera.m_matrices.viewMatrix = glm::inverse(cameraTransform);
	return camera;
}

struct CursorPositionGuard
{
	explicit CursorPositionGuard(GLFWwindow *window)
		: m_window(window)
	{
		glfwGetCursorPos(m_window, &m_x, &m_y);
	}

	~CursorPositionGuard()
	{
		glfwSetCursorPos(m_window, m_x, m_y);
	}

	GLFWwindow *m_window;
	double m_x = 0.0;
	double m_y = 0.0;
};
}

TEST_CASE("SceneTemplate adds and removes meshes", "[scene][mesh]")
{
	Mesh mesh(trianglePolygons, triangleVertices);
	Scene scene = makeScene();

	REQUIRE_THROWS_AS(scene.addMesh(nullptr), std::invalid_argument);

	scene.addMesh(&mesh);
	REQUIRE(mesh.m_faceIndex != nullptr);
	REQUIRE(scene.getAllMeshes() == std::vector<Mesh *>{&mesh});

	scene.addMesh(&mesh);
	REQUIRE(scene.getAllMeshes().size() == 1);

	scene.removeMesh(&mesh);
	REQUIRE(mesh.m_faceIndex == nullptr);
	REQUIRE(scene.getAllMeshes().empty());
	REQUIRE(queryRay(scene, {-0.25f, -0.25f, -2.0f}).empty());

	scene.removeMesh(&mesh);
	scene.removeMesh(nullptr);
}

TEST_CASE("SceneTemplate adds and removes face-index entries", "[scene][face]")
{
	Mesh mesh(trianglePolygons, triangleVertices);
	Scene scene = makeScene();
	ExtendedFace *face = mesh.m_halfEdgeStructure->m_faces.front();

	REQUIRE_THROWS_AS(scene.addFace(nullptr, face), std::invalid_argument);
	REQUIRE_THROWS_AS(scene.addFace(&mesh, nullptr), std::invalid_argument);
	REQUIRE_THROWS_AS(scene.addFace(&mesh, face), std::logic_error);

	scene.addMesh(&mesh);
	scene.removeFace(&mesh, face);
	REQUIRE(queryRay(scene, {-0.25f, -0.25f, -2.0f}).empty());

	scene.addFace(&mesh, face);
	auto hits = queryRay(scene, {-0.25f, -0.25f, -2.0f});
	REQUIRE(hits.size() == 1);
	REQUIRE(hits.front() == (FaceRef{&mesh, face}));

	scene.removeFace(nullptr, face);
	scene.removeMesh(&mesh);
}

TEST_CASE("SceneTemplate updates mesh and face spatial indices", "[scene][update]")
{
	Mesh mesh(trianglePolygons, triangleVertices);
	Scene scene = makeScene();
	scene.addMesh(&mesh);

	for (ExtendedVertex *vertex : mesh.m_halfEdgeStructure->m_vertices)
	{
		vertex->m_position.x += 20.0f;
	}

	REQUIRE_THROWS_AS(scene.updateMesh(nullptr), std::logic_error);
	scene.updateMesh(&mesh);

	REQUIRE(mesh.m_meshBounds.getMinBoundsPos().x == Catch::Approx(19.0f));
	REQUIRE(mesh.m_meshBounds.getMaxBoundsPos().x == Catch::Approx(21.0f));
	REQUIRE(queryRay(scene, {-0.25f, -0.25f, -2.0f}).empty());

	const auto hits = queryRay(scene, {19.75f, -0.25f, -2.0f});
	REQUIRE(hits.size() == 1);
	REQUIRE(hits.front().first == &mesh);

	for (ExtendedVertex *vertex : mesh.m_halfEdgeStructure->m_vertices)
	{
		vertex->m_position.x += 5.0f;
	}
	scene.updateMesh(&mesh, false);
	REQUIRE(mesh.m_meshBounds.getMinBoundsPos().x == Catch::Approx(24.0f));
	REQUIRE(mesh.m_meshBounds.getMaxBoundsPos().x == Catch::Approx(26.0f));
	REQUIRE(queryRay(scene, {24.75f, -0.25f, -2.0f}).empty());

	scene.removeMesh(&mesh);
}

TEST_CASE("SceneTemplate queries meshes and faces with rays, bounds, and spheres", "[scene][query]")
{
	Mesh mesh(trianglePolygons, triangleVertices);
	Scene scene = makeScene();
	scene.addMesh(&mesh);

	const auto rayHits = queryRay(scene, {-0.25f, -0.25f, -2.0f});
	REQUIRE(rayHits.size() == 1);
	REQUIRE(rayHits.front().first == &mesh);

	std::vector<FaceRef> boundsHits;
	scene.query(
		AABBBoundingRegion{{-0.35f, -0.35f, -0.1f}, {-0.15f, -0.15f, 0.1f}},
		boundsHits);
	REQUIRE(boundsHits.size() == 1);
	REQUIRE(boundsHits.front() == rayHits.front());

	std::vector<FaceRef> sphereHits;
	scene.query(Sphere{{0.0f, 0.0f, 0.0f}, 0.1f}, sphereHits);
	REQUIRE(sphereHits.size() == 1);
	REQUIRE(sphereHits.front() == rayHits.front());

	REQUIRE(queryRay(scene, {3.0f, 3.0f, -2.0f}).empty());
	scene.removeMesh(&mesh);
}

TEST_CASE("SceneUtilities rejects missing camera or window", "[scene][utilities]")
{
	Scene scene = makeScene();
	Camera camera{{0.0f, 0.0f, -3.0f}};
	Window *window = nullptr;

	REQUIRE_THROWS_AS(
		SceneUtilities::retClosestHitData(nullptr, window, scene),
		std::invalid_argument);
	REQUIRE_THROWS_AS(
		SceneUtilities::retDataUnderSelectionRectangle(
			{{40.0f, 40.0f}, {60.0f, 60.0f}},
			&camera,
			window,
			scene),
		std::invalid_argument);
}

TEST_CASE("SceneUtilities returns closest hit", "[scene][utilities]")
{
	Window &window = Application::getWindow();
	GLFWwindow *nativeWindow = window.getWindowHandle();
	REQUIRE(nativeWindow != nullptr);
	CursorPositionGuard cursorPositionGuard{nativeWindow};
	glfwSetCursorPos(
		nativeWindow,
		window.getScreenWidth() / 2.0,
		window.getScreenHeight() / 2.0);

	auto farVertices = triangleVertices;
	auto farOffset = glm::vec3{0.0f, 0.0f, 1.0f};
	for (glm::vec3 &vertex : farVertices)
	{
		vertex += farOffset;
	}
	auto nearVertices = triangleVertices;
	auto nearOffset = glm::vec3{0.0f, 0.0f, -1.0f};	
	for (glm::vec3 &vertex : nearVertices)
	{
		vertex += nearOffset;
	}

	Mesh farMesh(trianglePolygons, farVertices);
	Mesh nearMesh(trianglePolygons, nearVertices);
	Material farMaterial{"scene-test-far-material"};
	Material nearMaterial{"scene-test-near-material"};
	populateRenderableFace(farMesh, farMaterial, farOffset);
	populateRenderableFace(nearMesh, nearMaterial, nearOffset);
	Scene scene = makeScene();
	scene.addMesh(&farMesh);
	scene.addMesh(&nearMesh);
	const glm::vec3 rayDirection{0.0f, 0.0f, 1.0f};
	Camera camera = makePickingCamera(window, rayDirection);

	const auto [closestHit, hitPosition] =
		SceneUtilities::retClosestHitData(&camera, &window, scene);
	REQUIRE(closestHit.first == &nearMesh);
	REQUIRE(closestHit.second == nearMesh.m_halfEdgeStructure->m_faces.front());
	const float hitDistance = 2.0f / rayDirection.z;
	REQUIRE(hitPosition.x == Catch::Approx(rayDirection.x * hitDistance).margin(0.001f));
	REQUIRE(hitPosition.y == Catch::Approx(rayDirection.y * hitDistance).margin(0.001f));
	REQUIRE(hitPosition.z == Catch::Approx(-1.0f).margin(0.001f));

	camera.m_matrices.viewMatrix = glm::lookAt(
		glm::vec3{0.0f, 0.0f, -3.0f},
		glm::vec3{0.0f, 0.0f, -4.0f},
		glm::vec3{0.0f, 1.0f, 0.0f});
	const auto [noHit, noHitPosition] =
		SceneUtilities::retClosestHitData(&camera, &window, scene);
	REQUIRE(noHit.first == nullptr);
	REQUIRE(noHit.second == nullptr);
	REQUIRE(noHitPosition == glm::vec3{0.0f});

	scene.removeMesh(&nearMesh);
	scene.removeMesh(&farMesh);
}

TEST_CASE("SceneUtilities accepts a ray on a triangle edge", "[scene][utilities][edge]")
{
	Window &window = Application::getWindow();
	GLFWwindow *nativeWindow = window.getWindowHandle();
	REQUIRE(nativeWindow != nullptr);
	CursorPositionGuard cursorPositionGuard{nativeWindow};
	glfwSetCursorPos(
		nativeWindow,
		window.getScreenWidth() / 2.0,
		window.getScreenHeight() / 2.0);

	Mesh mesh(trianglePolygons, triangleVertices);
	Material material{"scene-test-edge-hit-material"};
	populateRenderableFace(mesh, material, {});
	Scene scene = makeScene();
	scene.addMesh(&mesh);

	const glm::vec3 rayOrigin{0.0f, 0.0f, -3.0f};
	const glm::vec3 edgeHitPosition{0.0f, -1.0f, 0.0f};
	const glm::vec3 rayDirection = edgeHitPosition - rayOrigin;
	Camera camera = makePickingCamera(window, rayDirection);
	const Ray ray = Ray::fromMousePos(camera, window);
	const glm::vec3 normalizedDirection = glm::normalize(rayDirection);
	REQUIRE(ray.direction.x == Catch::Approx(normalizedDirection.x).margin(0.001f));
	REQUIRE(ray.direction.y == Catch::Approx(normalizedDirection.y).margin(0.001f));
	REQUIRE(ray.direction.z == Catch::Approx(normalizedDirection.z).margin(0.001f));

	const auto [hit, position] =
		SceneUtilities::retClosestHitData(&camera, &window, scene);
	const FaceRef expectedHit{&mesh, mesh.m_halfEdgeStructure->m_faces.front()};
	REQUIRE(hit == expectedHit);
	REQUIRE(position.x == Catch::Approx(edgeHitPosition.x).margin(0.001f));
	REQUIRE(position.y == Catch::Approx(edgeHitPosition.y).margin(0.001f));
	REQUIRE(position.z == Catch::Approx(edgeHitPosition.z).margin(0.001f));

	scene.removeMesh(&mesh);
}

TEST_CASE("SceneUtilities rejects a ray just outside a triangle edge", "[scene][utilities][edge]")
{
	Window &window = Application::getWindow();
	GLFWwindow *nativeWindow = window.getWindowHandle();
	REQUIRE(nativeWindow != nullptr);
	CursorPositionGuard cursorPositionGuard{nativeWindow};
	glfwSetCursorPos(
		nativeWindow,
		window.getScreenWidth() / 2.0,
		window.getScreenHeight() / 2.0);

	Mesh mesh(trianglePolygons, triangleVertices);
	Material material{"scene-test-edge-miss-material"};
	populateRenderableFace(mesh, material, {});
	Scene scene = makeScene();
	scene.addMesh(&mesh);

	const glm::vec3 rayDirection{0.34f, 0.0f, 1.0f};
	Camera camera = makePickingCamera(window, rayDirection); //P on z=0 is (0.500001, 0, 0)
	const auto [hit, position] =
		SceneUtilities::retClosestHitData(&camera, &window, scene);
	REQUIRE(hit.first == nullptr);
	REQUIRE(hit.second == nullptr);
	REQUIRE(position == glm::vec3{0.0f});

	scene.removeMesh(&mesh);
}

TEST_CASE("SceneUtilities rectangle-selected faces", "[scene][utilities]")
{
	Window &window = Application::getWindow();
	const std::vector<glm::vec3> smallTriangleVertices{
		{-0.1f, -0.1f, 0.0f},
		{0.1f, -0.1f, 0.0f},
		{0.0f, 0.1f, 0.0f}};
	const auto offsetToScreenPosition = [&window](float screenX)
	{
		const float depth = 2.0f;
		const float halfFovRadians = glm::radians(45.0f) * 0.5f;
		const float normalizedX = 2.0f * screenX / window.getScreenWidth() - 1.0f;
		return glm::vec3{
			normalizedX * depth * std::tan(halfFovRadians) *
				static_cast<float>(window.getScreenWidth()) / window.getScreenHeight(),
			0.0f,
			-1.0f};
	};
	const glm::vec3 leftOffset = offsetToScreenPosition(30.0f);
	const glm::vec3 centerOffset = offsetToScreenPosition(50.0f);
	const glm::vec3 rightOffset = offsetToScreenPosition(70.0f);
	const auto verticesAtOffset = [&smallTriangleVertices](const glm::vec3 &offset)
	{
		auto vertices = smallTriangleVertices;
		for (glm::vec3 &vertex : vertices)
		{
			vertex += offset;
		}
		return vertices;
	};
	Mesh leftMesh(trianglePolygons, verticesAtOffset(leftOffset));
	Mesh centerMesh(trianglePolygons, verticesAtOffset(centerOffset));
	Mesh rightMesh(trianglePolygons, verticesAtOffset(rightOffset));
	Material leftMaterial{"scene-test-left-material"};
	Material centerMaterial{"scene-test-center-material"};
	Material rightMaterial{"scene-test-right-material"};
	populateRenderableFace(leftMesh, leftMaterial, leftOffset, smallTriangleVertices);
	populateRenderableFace(centerMesh, centerMaterial, centerOffset, smallTriangleVertices);
	populateRenderableFace(rightMesh, rightMaterial, rightOffset, smallTriangleVertices);
	Scene scene = makeScene();
	scene.addMesh(&leftMesh);
	scene.addMesh(&centerMesh);
	scene.addMesh(&rightMesh);
	Camera camera{{0.0f, 0.0f, -3.0f}};
	camera.m_matrices.viewMatrix = glm::lookAt(
		camera.getState().position,
		glm::vec3{0.0f, 0.0f, 0.0f},
		glm::vec3{0.0f, 1.0f, 0.0f});
	camera.m_matrices.perspectiveMatrix = glm::perspective(
		glm::radians(45.0f),
		static_cast<float>(window.getScreenWidth()) / window.getScreenHeight(),
		0.1f,
		100.0f);

	const auto largeRectangleHits = SceneUtilities::retDataUnderSelectionRectangle(
		{{10.0f, 10.0f}, {90.0f, 90.0f}},
		&camera,
		&window,
		scene);
	REQUIRE(largeRectangleHits.size() == 3);
	REQUIRE(std::find(
				largeRectangleHits.begin(),
				largeRectangleHits.end(),
				FaceRef{&leftMesh, leftMesh.m_halfEdgeStructure->m_faces.front()}) !=
			largeRectangleHits.end());
	REQUIRE(std::find(
				largeRectangleHits.begin(),
				largeRectangleHits.end(),
				FaceRef{&centerMesh, centerMesh.m_halfEdgeStructure->m_faces.front()}) !=
			largeRectangleHits.end());
	REQUIRE(std::find(
				largeRectangleHits.begin(),
				largeRectangleHits.end(),
				FaceRef{&rightMesh, rightMesh.m_halfEdgeStructure->m_faces.front()}) !=
			largeRectangleHits.end());

	const auto smallRectangleHits = SceneUtilities::retDataUnderSelectionRectangle(
		{{40.0f, 40.0f}, {60.0f, 60.0f}},
		&camera,
		&window,
		scene);
	const FaceRef centerFace{
		&centerMesh,
		centerMesh.m_halfEdgeStructure->m_faces.front()};
	REQUIRE(smallRectangleHits.size() == 1);
	REQUIRE(smallRectangleHits.front() == centerFace);

	scene.removeMesh(&leftMesh);
	scene.removeMesh(&centerMesh);
	scene.removeMesh(&rightMesh);
}

TEST_CASE(
	"SceneUtilities rectangle selection includes triangle corner and edge contacts",
	"[scene][utilities][edge]")
{
	Window &window = Application::getWindow();
	const auto cornerVertices = triangleAtScreenPositions(
		window,
		{glm::vec2{49.0f, 50.0f}, glm::vec2{50.0f, 49.0f}, glm::vec2{50.0f, 50.0f}});
	const auto edgeVertices = triangleAtScreenPositions(
		window,
		{glm::vec2{50.0f, 52.0f}, glm::vec2{50.0f, 68.0f}, glm::vec2{49.0f, 60.0f}});
	Mesh cornerMesh(trianglePolygons, cornerVertices);
	Mesh edgeMesh(trianglePolygons, edgeVertices);
	Material cornerMaterial{"scene-test-corner-contact-material"};
	Material edgeMaterial{"scene-test-edge-contact-material"};
	populateRenderableFace(cornerMesh, cornerMaterial, {}, cornerVertices);
	populateRenderableFace(edgeMesh, edgeMaterial, {}, edgeVertices);
	Scene scene = makeScene();
	scene.addMesh(&cornerMesh);
	scene.addMesh(&edgeMesh);

	Camera camera{{0.0f, 0.0f, -3.0f}};
	camera.m_matrices.viewMatrix = glm::lookAt(
		camera.getState().position,
		glm::vec3{0.0f, 0.0f, 0.0f},
		glm::vec3{0.0f, 1.0f, 0.0f});
	camera.m_matrices.perspectiveMatrix = glm::perspective(
		glm::radians(45.0f),
		static_cast<float>(window.getScreenWidth()) / window.getScreenHeight(),
		0.1f,
		100.0f);

	const auto hits = SceneUtilities::retDataUnderSelectionRectangle(
		{{50.0f, 50.0f}, {70.0f, 70.0f}},
		&camera,
		&window,
		scene);
	REQUIRE(hits.size() == 2);
	REQUIRE(std::find(
				hits.begin(),
				hits.end(),
				FaceRef{&cornerMesh, cornerMesh.m_halfEdgeStructure->m_faces.front()}) !=
			hits.end());
	REQUIRE(std::find(
				hits.begin(),
				hits.end(),
				FaceRef{&edgeMesh, edgeMesh.m_halfEdgeStructure->m_faces.front()}) !=
			hits.end());

	scene.removeMesh(&cornerMesh);
	scene.removeMesh(&edgeMesh);
}

TEST_CASE(
	"SceneUtilities rectangle selection rejects triangles just outside its corner and edge",
	"[scene][utilities][edge]")
{
	Window &window = Application::getWindow();
	const auto cornerMissVertices = triangleAtScreenPositions(
		window,
		{glm::vec2{49.99f, 49.99f}, glm::vec2{49.0f, 49.99f}, glm::vec2{49.99f, 49.0f}});
	const auto edgeMissVertices = triangleAtScreenPositions(
		window,
		{glm::vec2{49.99f, 52.0f}, glm::vec2{49.99f, 68.0f}, glm::vec2{49.0f, 60.0f}});
	Mesh cornerMissMesh(trianglePolygons, cornerMissVertices);
	Mesh edgeMissMesh(trianglePolygons, edgeMissVertices);
	Material cornerMissMaterial{"scene-test-corner-miss-material"};
	Material edgeMissMaterial{"scene-test-edge-miss-material"};
	populateRenderableFace(cornerMissMesh, cornerMissMaterial, {}, cornerMissVertices);
	populateRenderableFace(edgeMissMesh, edgeMissMaterial, {}, edgeMissVertices);
	Scene scene = makeScene();
	scene.addMesh(&cornerMissMesh);
	scene.addMesh(&edgeMissMesh);

	Camera camera{{0.0f, 0.0f, -3.0f}};
	camera.m_matrices.viewMatrix = glm::lookAt(
		camera.getState().position,
		glm::vec3{0.0f, 0.0f, 0.0f},
		glm::vec3{0.0f, 1.0f, 0.0f});
	camera.m_matrices.perspectiveMatrix = glm::perspective(
		glm::radians(45.0f),
		static_cast<float>(window.getScreenWidth()) / window.getScreenHeight(),
		0.1f,
		100.0f);

	const auto hits = SceneUtilities::retDataUnderSelectionRectangle(
		{{50.0f, 50.0f}, {70.0f, 70.0f}},
		&camera,
		&window,
		scene);
	REQUIRE(hits.empty());

	scene.removeMesh(&cornerMissMesh);
	scene.removeMesh(&edgeMissMesh);
}
