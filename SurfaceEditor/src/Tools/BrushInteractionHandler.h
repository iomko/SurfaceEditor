#pragma once
#include <iostream>

#include "InteractionHandler.h"
#include "../Commands/CmdProperties/CmdProperties.h"
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
		/*
		Sphere sphere{ m_currentHitPoint, m_currentRadius };
		Ray ray = Ray::fromMousePos(*ViewPortsHolderContext::m_camera, ViewPortsHolderContext::m_camera->m_matrices.perspectiveMatrix,
			ViewPortsHolderContext::m_camera->m_matrices.viewMatrix, *ViewPortsHolderContext::m_window);

		if(!Ray::intersectsSphere(ray.origin, ray.direction, sphere))
		{
			BrushToolCommand* brushToolCommand = CommandRegistry::getCommand<BrushToolCommand>();
			OctreeNodeDataParams oParams;
			brushToolCommand->execute(iParams, oParams);

			if(oParams.meshFacePair.first != nullptr)
			{
				m_currentHitPoint = oParams.hitPoint;
				m_currentRadius = iParams.radius;
			}
		}
		*/
		
		Sphere sphere{ m_currentHitPoint, m_currentRadius };

		RetClosestOctreeDataCallable retClosestDataCallable;
		OctreeNodeDataParams oParams;
		retClosestDataCallable.invoke(oParams);

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
