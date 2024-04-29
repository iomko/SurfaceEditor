module;
#include <unordered_set>

#include "../Core/Input.h"
#include "../Renderer/Renderer.h"
#include "../DataStructures/HalfEdge.h"
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"

//for now
//#include "../Mesh.h"

export module LayerSystem.Layer.ViewPortLayer;
import LayerSystem.Layer;
import Scene;
import Camera;

export class ViewPortLayer : public Layer
{
private:
	float m_deltaTime = 0.0f;
	Camera* m_activeCamera = nullptr;
	Scene* m_scene = nullptr;

	std::unordered_set<HalfEdgeDS::HalfEdgeMesh*> m_selectedMeshesID;

public:
	Shader* m_meshShader = nullptr;
	Shader* m_normalsShader = nullptr;
	Shader* m_shaderSingleColor = nullptr;
	Shader* m_pointsShader = nullptr;
	Shader* m_linesShader = nullptr;

	Shader* m_shaderBlit = nullptr;
	Shader* m_shaderDilation = nullptr;

public:


	const unsigned int SCR_WIDTH = 1600;
	const unsigned int SCR_HEIGHT = 900;



	struct SimpleRenderTarget {
		unsigned int fbo{ 0 };               /// framebuffer id
		unsigned int colorTexId{ 0 };        /// colour buffer id
		unsigned int depthStencilTexId{ 0 }; /// depth and stencil buffer id
	};


	SimpleRenderTarget createRenderTarget() {
		unsigned int fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		SimpleRenderTarget rt;
		rt.fbo = fbo;

		{
			unsigned int texId;
			glGenTextures(1, &texId);
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
				texId, 0);
			rt.colorTexId = texId;
		}

		{
			unsigned int texId;
			glGenTextures(1, &texId);
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, SCR_WIDTH, SCR_HEIGHT, 0,
				GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
				GL_TEXTURE_2D, texId, 0);

			rt.depthStencilTexId = texId;
		}

		return rt;
	}

	SimpleRenderTarget m_targetA;
	SimpleRenderTarget m_targetB;
	

	Command* m_currentSelectedCommand = nullptr;
public:
	ViewPortLayer(const std::string& name, Camera* m_active_camera, Scene* scene)
		: Layer(name), m_activeCamera(m_active_camera), m_scene(scene)
	{
		Renderer::init();

		m_targetA = createRenderTarget();
		m_targetB = createRenderTarget();

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	Camera* getCamera()
	{
		return m_activeCamera;
	}

	Scene* getScene()
	{
		return m_scene;
	}

	void onUpdate() override
	{
		updateCameraMovement();
	}
	//unhide later
	void render()
	{



	}

	void updateDeltaTime(float deltaTime)
	{
		m_deltaTime = deltaTime;
	}

	void updateCameraDirection(Event& event)
	{
		if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
		{
			m_activeCamera->updateCameraDirection(Input::getMouseDx(), Input::getMouseDy());
			event.isHandled = true;
		}
	}

	void updateCameraMovement()
	{
		const float movementSpeed = 50.0f * m_deltaTime;
		if (Input::isKeyPressed(GLFW_KEY_W) == true)
		{
			m_activeCamera->updateCameraPosition(CameraMovement::FORWARD, movementSpeed);
			//std::cout << "camera moved forward" << std::endl;
		}
		if (Input::isKeyPressed(GLFW_KEY_S) == true)
		{
			m_activeCamera->updateCameraPosition(CameraMovement::BACKWARD, movementSpeed);
			//std::cout << "camera moved backward" << std::endl;
		}
		if (Input::isKeyPressed(GLFW_KEY_A) == true)
		{
			m_activeCamera->updateCameraPosition(CameraMovement::LEFT, movementSpeed);
			//std::cout << "camera moved left" << std::endl;
		}
		if (Input::isKeyPressed(GLFW_KEY_D) == true)
		{
			m_activeCamera->updateCameraPosition(CameraMovement::RIGHT, movementSpeed);
			//std::cout << "camera moved right" << std::endl;
		}
	}
	void onEvent(Event& event) override
	{
		if (event.getType() == EventType::MouseButtonPress)
		{
			
			if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
			{
				if (m_currentSelectedCommand != nullptr)
				{
					m_currentSelectedCommand->execute();
				}
			}
		}

		if(event.getType() == EventType::MouseButtonPress || event.getType() == EventType::MouseMove)
		{
			updateCameraDirection(event);
		}
	}

	//unhide later
	void deleteSelectedFacesOfSelectedMeshes()
	{
		/*
		std::vector<HalfEdgeDS::HalfEdgeMesh*> potentiallyDeletedMeshesID;
		for (const auto& meshID : m_selectedMeshesID)
		{
			// Create a list of faces to erase
			std::vector<HalfEdgeDS::Face*> facesToDelete;

			const auto& facesOfMesh = m_scene->m_meshesFaceOctreeMap.find(meshID);
			//but we need to only go through selected faces of that mesh.
			for (auto& selectedFace : meshID->getSelectedFaces())
			{
				const auto& octreesIDsAssociatedWithFace = facesOfMesh->second.find(selectedFace)->second;

				for (const auto& octreeID : octreesIDsAssociatedWithFace)
				{
					auto& foundOctree = m_scene->m_halfEdgeMeshOctrees.find(octreeID)->second;

					foundOctree.removeData(selectedFace);
					if (foundOctree.leafs.empty())
					{
						//ak je octree prazdny tak ho vymazat z m_halfEdgeMeshOctrees
						m_scene->m_halfEdgeMeshOctrees.erase(octreeID);
					}
				}
				facesToDelete.push_back(selectedFace);
			}

			// Erase collected faces
			for (const auto& face : facesToDelete)
			{
				face->getMesh().eraseSelectedFace(face);
				facesOfMesh->second.erase(face);
			}
			if (facesOfMesh->second.empty())
			{
				potentiallyDeletedMeshesID.push_back(meshID);
			}
		}
		for (const auto& meshID : potentiallyDeletedMeshesID)
		{
			m_selectedMeshesID.erase(meshID);
		}
		*/
	}

	/*
	void deleteSelectedMeshes()
	{
		for (const auto& meshID : m_selectedMeshesID)
		{
			const auto& faceOctreesIter = m_scene->m_meshesFaceOctreeMap.find(meshID)->second;
			
			for (const auto& faceOctrees : faceOctreesIter) {

				for (const auto& octreeID : faceOctrees.second)
				{
					auto& foundOctree = m_scene->m_halfEdgeMeshOctrees.find(octreeID)->second;
					foundOctree.removeData(faceOctrees.first);
					if(foundOctree.leafs.empty())
					{
						m_scene->m_halfEdgeMeshOctrees.erase(octreeID);
					}
				}
			}
			m_scene->m_meshesFaceOctreeMap.erase(meshID);
		}
		if(!m_selectedMeshesID.empty())
		{
			m_selectedMeshesID.clear();
		}

	}
	*/
	//unhide later
	HalfEdgeDS::Face* deselectFace(const Ray& ray)
	{
		
		HalfEdgeDS::Face* retrievedFace = nullptr;
		/*
		findFace(ray, retrievedFace);

		if (retrievedFace != nullptr)
		{
			auto itMesh = m_selectedMeshesID.find(&retrievedFace->getMesh());
			if (itMesh != m_selectedMeshesID.end()) {
				auto erased = retrievedFace->getMesh().eraseFaceFromSelectedFaces(retrievedFace);
				if(erased)
				{
					retrievedFace->getMesh().unmarkFace(retrievedFace, glm::vec3(0.5, 0.5, 0.5));
				}
			}
			
		}
		*/
		return retrievedFace;
	}
	//unhide later
	HalfEdgeDS::Face* selectFace(const Ray& ray)
	{
		
		//bolo by dobre prechadzat len cez octrees priradene selectnutym meshom. //zatial nebudem brat na toto ohlad a budem prechadzat cez vsetky octrees v scene
		HalfEdgeDS::Face* retrievedFace = nullptr;
		/*
		findFace(ray, retrievedFace);

		if (retrievedFace != nullptr)
		{
			auto itMesh = m_selectedMeshesID.find(&retrievedFace->getMesh());

			if (itMesh != m_selectedMeshesID.end()) {

				auto added = retrievedFace->getMesh().addFaceToSelectedFaces(retrievedFace);
				if(added)
				{
					retrievedFace->getMesh().markFace(retrievedFace, glm::vec3(1.0f, 0.7f, 0.2f));
				}
			} else
			{
				retrievedFace = nullptr;
			}
		}
		*/
		return retrievedFace;
	}
	//unhide later
	HalfEdgeDS::HalfEdgeMesh* selectMesh(const Ray& ray)
	{
		
		HalfEdgeDS::HalfEdgeMesh* retrievedMesh = nullptr;
		/*
		findMesh(ray, retrievedMesh);

		if(retrievedMesh != nullptr)
		{
			m_selectedMeshesID.insert(retrievedMesh);
			retrievedMesh->markSelectedFaces(glm::vec3(1.0f, 0.7f, 0.2f));
		}
		*/
		return retrievedMesh;
	}
	//unhide later
	HalfEdgeDS::HalfEdgeMesh* deselectMesh(const Ray& ray)
	{
		HalfEdgeDS::HalfEdgeMesh* retrievedMesh = nullptr;
		/*
		findMesh(ray, retrievedMesh);

		if(retrievedMesh != nullptr)
		{
			auto it = m_selectedMeshesID.find(retrievedMesh);
			if (it != m_selectedMeshesID.end()) {
				retrievedMesh->unmarkSelectedFaces(glm::vec3(0.5, 0.5, 0.5));
				m_selectedMeshesID.erase(it);
			}
		}
		*/
		return retrievedMesh;
	}

};
