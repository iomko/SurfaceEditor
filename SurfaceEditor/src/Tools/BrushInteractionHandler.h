#pragma once
#include <iostream>

#include "InteractionHandler.h"
#include "../Params/OperationParams.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/BrushToolCommand.h"

class BrushInteractionHandler : public InteractionHandler<BrushToolParams>
{
public:
	void onBegin(const BrushToolParams& iParams) override
	{
		std::cout << "OnBegin executed" << std::endl;
		BrushToolCommand* brushToolCommand = CommandRegistry::getCommand<BrushToolCommand>();
		OctreeNodeDataParams oParams;
		brushToolCommand->execute(iParams, oParams);

		m_currentHitPoint = oParams.hitPoint;
		m_currentRadius = iParams.radius;
	}
	void onUpdate(const BrushToolParams& iParams) override
	{
		Sphere sphere{ m_currentHitPoint, m_currentRadius };

		Camera* camera = ViewPortsHolderContext::m_camera;
		Window* window = ViewPortsHolderContext::m_window;
		Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;
		std::pair<SceneRes::MeshFacePair, glm::vec3> meshFaceHitPair = SceneUtilities::retClosestHitData(camera, window, scene->m_res);

		OctreeNodeDataParams oParams;
		oParams.meshFacePair = meshFaceHitPair.first;
		oParams.hitPoint = meshFaceHitPair.second;

		if(!sphere.containsPoint(oParams.hitPoint))
		{
			BrushToolCommand* brushToolCommand = CommandRegistry::getCommand<BrushToolCommand>();
			brushToolCommand->execute(iParams, oParams);

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
