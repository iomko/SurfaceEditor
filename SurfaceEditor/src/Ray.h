#pragma once

#include <glm/glm.hpp>

#include "Sphere.h"
#include "Scene/Camera.h"
#include "Core/Window.h"

class Ray {
public:
	Ray(){}

	Ray(const glm::vec3& origin, const glm::vec3& direction) {
		this->origin = origin;
		this->direction = direction;
		this->inverseDirection = 1.0f / this->direction;
		sign[0] = (this->inverseDirection.x < 0);
		sign[1] = (this->inverseDirection.y < 0);
		sign[2] = (this->inverseDirection.z < 0);
	}

	static Ray getCameraRay(Camera& camera)
	{
		return Ray(camera.getState().position, camera.getState().frontVector);
	}

	static Ray fromMousePos(Camera& camera, Window& window)
	{
		double mouseX, mouseY;
		glfwGetCursorPos(window.getWindowHandle(), &mouseX, &mouseY);

		auto normalizedXCoord = (2.0 * mouseX) / window.getScreenWidth() - 1.0;
		auto normalizedYCoord = 1.0 - (2.0 * mouseY) / window.getScreenHeight();

		glm::vec4 clip = glm::vec4(normalizedXCoord, normalizedYCoord, -1.0, 1.0);

		glm::mat4 inversePerspective = glm::inverse(camera.m_matrices.perspectiveMatrix);
		glm::vec4 cameraSpace = inversePerspective * clip;
		cameraSpace[2] = -1.0;
		cameraSpace[3] = 0.0;

		glm::vec4 worldSpace = glm::inverse(camera.m_matrices.viewMatrix) * cameraSpace;

		glm::vec3 rayPosition = glm::vec3(worldSpace);
		glm::vec3 normalizedRayPosition = glm::normalize(rayPosition);


		glm::vec3 rayStart = camera.getState().position;


		return Ray(rayStart, normalizedRayPosition);
	}

	static bool intersectsSphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const Sphere& sphere)
	{
		glm::vec3 L = sphere.position - rayOrigin;
		float tca = glm::dot(L, rayDir);
		if (tca < 0) return false;

		float d2 = glm::dot(L, L) - tca * tca;
		float radius2 = sphere.radius * sphere.radius;

		return d2 <= radius2;
	}

	static bool intersectPlane(const glm::vec3& n, const glm::vec3& p0, const glm::vec3& l0, const glm::vec3& l, float& t)
	{
		float denom = glm::dot(n, l);
		if (abs(denom) > 0.0001f) // your favorite epsilon
		{
			t = glm::dot((p0 - l0), n) / denom;
			if (t >= 0) return true; // you might want to allow an epsilon here too
		}
		return false;

	}

	static bool intersectsTriangle(const glm::vec3& orig, const glm::vec3& dir, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t)
	{
		// compute the plane's normal
		glm::vec3 v0v1 = v1 - v0;
		glm::vec3 v0v2 = v2 - v0;
		// no need to normalize
		glm::vec3 N = glm::cross(v0v1, v0v2); // N
		float area2 = N.length();

		// Step 1: finding P

		// check if the ray and plane are parallel.
		float NdotRayDirection = glm::dot(N, dir);
		if (fabs(NdotRayDirection) < 1e-6) // almost 0
			return false; // they are parallel, so they don't intersect! 

		// compute d parameter using equation 2
		float d = -glm::dot(N, v0);
		// compute t (equation 3)
		t = -(glm::dot(N, orig) + d) / NdotRayDirection;

		// check if the triangle is behind the ray
		if (t < 0) return false; // the triangle is behind

		// compute the intersection position using equation 1
		glm::vec3 P = orig + t * dir;

		// Step 2: inside-outside test
		glm::vec3 C; // vector perpendicular to triangle's plane

		// edge 0
		glm::vec3 edge0 = v1 - v0;
		glm::vec3 vp0 = P - v0;
		C = glm::cross(edge0, vp0);
		if (glm::dot(N, C) < 0) return false; // P is on the right side

		// edge 1
		glm::vec3 edge1 = v2 - v1;
		glm::vec3 vp1 = P - v1;
		C = glm::cross(edge1, vp1);
		if (glm::dot(N, C) < 0)  return false; // P is on the right side

		// edge 2
		glm::vec3 edge2 = v0 - v2;
		glm::vec3 vp2 = P - v2;
		C = glm::cross(edge2, vp2);
		if (glm::dot(N, C) < 0) return false; // P is on the right side;

		return true; // this ray hits the triangle
	}


	glm::vec3 origin{};
	glm::vec3 direction{};
	glm::vec3 inverseDirection{};
	int sign[3]{};
};
