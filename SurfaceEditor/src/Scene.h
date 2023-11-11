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
	std::vector<std::pair<Mesh, Octree>> meshOctreePairs;
public:
	void renderScene(const Camera& camera)
	{
		//na to aby sme vykreslili scenu tak musime prejst cez vsetky renderovacie komponenty, ktore sa nachadzaju v scene
		//najskor prejdeme vsetkymi meshami
		for (const auto meshOctreePair &: meshOctreePairs)
		{
			
		}
	}
	void updateScene()
	{
		
	}
};