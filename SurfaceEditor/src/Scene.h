#pragma once

#include <vector>
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "Octree.h"
#include "Renderer.h"

class Scene
{
private:
	//cameras
	std::vector<Camera> cameras;
	Camera& activeCamera;
	//shaders
	std::vector<Shader> shaders;
	Shader& activeShader;
	//meshes
public:
	std::vector<std::pair<Mesh, Octree>> meshOctreePairs;
public:
	void renderScene(const Camera& camera)
	{
		for (const auto meshOctreePair &: meshOctreePairs)
		{
			
		}
	}
	void updateScene()
	{
		
	}
};