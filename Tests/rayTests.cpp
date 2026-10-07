#include "../src/Ray.h"
#include "../src/Primitives/Sphere.h"
#include "../src/Scene/Camera.h"
#include "../src/Core/Window.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("Ray constructor sets fields and sign", "[ray][basic]") {
    glm::vec3 o(0.0f, 0.0f, 0.0f);
    glm::vec3 d(2.0f, 1.0f, 0.5f);
    Ray r(o, d);

    REQUIRE(r.origin == o);
    REQUIRE(r.direction == d);
    REQUIRE(r.inverseDirection.x == Catch::Approx(1.0f / d.x));
    REQUIRE(r.inverseDirection.y == Catch::Approx(1.0f / d.y));
    REQUIRE(r.inverseDirection.z == Catch::Approx(1.0f / d.z));
    REQUIRE((r.sign[0] == (r.inverseDirection.x < 0)));
}

TEST_CASE("Ray intersects sphere correctly", "[ray][sphere]") {
    Sphere s;
    s.position = glm::vec3(5,0,0);
    s.radius = 1.0f;

    glm::vec3 o(0,0,0);
    glm::vec3 dir = glm::normalize(glm::vec3(1,0,0));

    REQUIRE(Ray::intersectsSphere(o, dir, s) == true);

    s.position = glm::vec3(-5,0,0);
    REQUIRE(Ray::intersectsSphere(o, dir, s) == false);
}

TEST_CASE("Ray intersectPlane handles hit and parallel cases", "[ray][plane]") {
    glm::vec3 n(0,1,0);
    glm::vec3 p0(0,0,0); // y=0 plane

    glm::vec3 l0(0,1,0);
    glm::vec3 l(0,-1,0);
    float t = 0.0f;
    REQUIRE(Ray::intersectPlane(n, p0, l0, l, t) == true);
    REQUIRE(t == Catch::Approx(1.0f));

    // Parallel ray
    glm::vec3 lpar(1,0,0);
    REQUIRE(Ray::intersectPlane(n, p0, l0, lpar, t) == false);
}

TEST_CASE("Ray intersects triangle correctly", "[ray][triangle]") {
    glm::vec3 v0(0,0,0), v1(1,0,0), v2(0,1,0);

    glm::vec3 orig(0.1f,0.1f,-1.0f);
    glm::vec3 dir(0,0,1);
    float t = 0.0f;
    REQUIRE(Ray::intersectsTriangle(orig, dir, v0, v1, v2, t) == true);
    REQUIRE(t > 0);

    // Miss case
    glm::vec3 orig2(1.5f,1.5f,-1.0f);
    REQUIRE(Ray::intersectsTriangle(orig2, dir, v0, v1, v2, t) == false);
}

TEST_CASE("Ray getCameraRay returns camera origin and direction", "[ray][camera]") {
    Camera camera(glm::vec3(1.0f, 2.0f, 3.0f), glm::vec3(2.0f, 2.0f, 3.0f));
    Ray ray = Ray::getCameraRay(camera);

    REQUIRE(ray.origin == glm::vec3(1.0f, 2.0f, 3.0f));
    REQUIRE(ray.direction == camera.getState().frontVector);
}

TEST_CASE("Ray intersectPlane returns false for rays behind the plane", "[ray][plane][negative]") {
    glm::vec3 normal(0,1,0);
    glm::vec3 planePoint(0,0,0);
    glm::vec3 rayOrigin(0,-1,0);
    glm::vec3 rayDirection(0,-1,0);
    float t = 0.0f;

    REQUIRE(Ray::intersectPlane(normal, planePoint, rayOrigin, rayDirection, t) == false);
}

TEST_CASE("Ray fromPos uses camera matrices and window coordinates", "[ray][window]") {
    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    camera.m_matrices.viewMatrix = glm::lookAt(camera.getState().position, camera.getState().position + camera.getState().frontVector, camera.getState().worldUp);
    camera.m_matrices.perspectiveMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    Window window(100, 100, "unit_test");

    Ray ray = Ray::fromPos(camera, window, 50.0, 50.0);

    REQUIRE(ray.origin == camera.getState().position);
    REQUIRE(glm::length(ray.direction) > 0.0f);
}
