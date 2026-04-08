#pragma once
#include "InteractionHandler.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Input.h"
#include "../UI/SelectionRectangle.h"
#include "../ViewPortsController.h"

template<typename CommandConcept>
class SelectionHandler : public InteractionHandler<CommandConcept>
{
public:
	SelectionHandler(CommandConcept* command)
		: InteractionHandler<CommandConcept>(command)
	{ }

	void onBegin() override
	{
		auto mousePos = glm::vec2(Input::getMouseX(), Input::getMouseY());

		m_selectionRectanglePos.startPos = mousePos;
		m_selectionRectanglePos.endPos	 = mousePos;
		
		updateSelectionRectangle();
	}

	void onUpdate() override
	{
		auto mousePos = glm::vec2(Input::getMouseX(), Input::getMouseY());

		m_selectionRectanglePos.endPos = mousePos;

		updateSelectionRectangle();
	}

	void onEnd() override
	{
		ViewPortsHolderContext::s_selectionController->clearSelectionRectangle();
		SelectionToolParams toolParams{};

		bool isClick = m_selectionRectanglePos.startPos == m_selectionRectanglePos.endPos;
		if (isClick)
		{
			if (ViewPortsHolderContext::s_uiLayerController->clickedOnUiWindow(m_selectionRectanglePos.startPos))
			{
				return;
			}
			isSelectedByClick(toolParams);
		}
		else
		{
			isSelectedByRectangle(toolParams);
		}

		this->getCommand()->execute(toolParams);
	}

private:
	void updateSelectionRectangle()
	{
		const int windowWidth  = ViewPortsHolderContext::s_window->getScreenWidth();
		const int windowHeight = ViewPortsHolderContext::s_window->getScreenHeight();

		ViewPortsHolderContext::s_selectionController->updateSelectionRectangle(windowWidth, windowHeight, m_selectionRectanglePos);
	}

	void isSelectedByClick(SelectionToolParams& toolParams)
	{
		Camera* camera = ViewPortsHolderContext::s_camera.get();
		Window* window = ViewPortsHolderContext::s_window;
		Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

		const auto meshFaceHitPair = SceneUtilities::retClosestHitData(camera, window, scene->m_res);
		auto& meshFacePair = meshFaceHitPair.first;
		auto mesh = meshFacePair.first;

		//TODO FACE, VERTEX, EDGE
		if (mesh != nullptr)
		{
			toolParams.m_selectedData.push_back(meshFacePair);
			toolParams.m_select = true;
		}
		else
		{
			toolParams.m_select = false;
		}
	}

	void isSelectedByRectangle(SelectionToolParams& toolParams)
	{
		Camera* camera = ViewPortsHolderContext::s_camera.get();
		Window* window = ViewPortsHolderContext::s_window;
		Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

		//TODO FACE, VERTEX, EDGE
		toolParams.m_selectedData = SceneUtilities::retDataUnderSelectionRectangle(m_selectionRectanglePos, camera, window, scene->m_res);
		toolParams.m_select = !toolParams.m_selectedData.empty();
	}

private:
	RectanglePos m_selectionRectanglePos;
};