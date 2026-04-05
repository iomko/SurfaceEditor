#pragma once
#include <iostream>

#include "../../src/Tools/InteractionHandler.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Command.h"
#include "../../src/ViewPortsController.h"

class BrushInteractionHandler : public InteractionHandler<CommandConcept, BrushToolParams>
{
public:
	BrushInteractionHandler(CommandConcept* command)
		: InteractionHandler(command) {}

	void onBegin(const BrushToolParams& iParams) override
	{
		OctreeNodeDataParams oParams;
		this->getCommand()->execute(iParams, oParams);

		m_currentHitPoint = oParams.hitPoint;
		m_currentRadius = iParams.radius;
	}
	void onUpdate(const BrushToolParams& iParams) override
	{
		Sphere sphere{ m_currentHitPoint, m_currentRadius };

		Camera* camera = ViewPortsHolderContext::s_camera.get();
		Window* window = ViewPortsHolderContext::s_window;
		Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
		std::pair<SceneResources::MeshFacePair, glm::vec3> meshFaceHitPair = SceneUtilities::retClosestHitData(camera, window, scene->m_res);

		OctreeNodeDataParams oParams;
		oParams.meshFacePair = meshFaceHitPair.first;
		oParams.hitPoint = meshFaceHitPair.second;

		if(!sphere.containsPoint(oParams.hitPoint))
		{
			this->getCommand()->execute(iParams, oParams);

			if (oParams.meshFacePair.first != nullptr)
			{
				m_currentHitPoint = oParams.hitPoint;
				m_currentRadius = iParams.radius;
			}
		}
	}
	void onEnd(const BrushToolParams& iParams) override
	{
		std::cout << "Done working with the command." << std::endl;
	}

private:
	glm::vec3 m_currentHitPoint{};
	float m_currentRadius{};
};
