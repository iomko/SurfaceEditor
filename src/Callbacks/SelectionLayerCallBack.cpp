#include "SelectionLayerCallBack.h"
#include "CallbackRegister.h"
#include "../Tools/ToolRegistry.h"
#include "../Tools/ToolIDs.h"
#include "../ViewPortsController.h"
#include "../CsvFeatureExporter.h"
#include "../Ml/Models/TriangleSkewMlModel.h"

static AutoRegisterCallback<SelectionLayerCallBack> autoRegisterSelectionLayerCallBack;

bool SelectionLayerCallBack::isClick(const RectanglePos& rectanglePos)
{
	return glm::abs(rectanglePos.startPos.x - rectanglePos.endPos.x) < 1.0f &&
		   glm::abs(rectanglePos.endPos.y - rectanglePos.endPos.y) < 1.0f;
}

bool SelectionLayerCallBack::isSelectedByClick(const SelectionLayerParams& params, SelectionToolParams* toolParams)
{
	Camera* camera = ViewPortsHolderContext::s_camera;
    Window* window = ViewPortsHolderContext::s_window;
    Scene* scene   = ViewPortsHolderContext::s_viewPortsController->m_scene;

	const auto meshFaceHitPair = SceneUtilities::retClosestHitData(camera, window, scene->m_res);
	auto& meshFacePair = meshFaceHitPair.first;
	auto mesh = meshFacePair.first;
	auto face = meshFacePair.second;

	//TODO isEdgeSelected
	//TODO isVertexSelected
	if ((m_selectionMode == SelectionMode::MESH && mesh != nullptr) ||
		(m_selectionMode == SelectionMode::FACE && face != nullptr))
	{
		toolParams->m_selectedData.push_back(meshFacePair);
		return true;
	}
	return false;
}

bool SelectionLayerCallBack::isSelectedByRectangle(const SelectionLayerParams& params, SelectionToolParams* toolParams)
{
	Camera* camera = ViewPortsHolderContext::s_camera;
    Window* window = ViewPortsHolderContext::s_window;
    Scene* scene   = ViewPortsHolderContext::s_viewPortsController->m_scene;

	toolParams->m_selectedData = SceneUtilities::retDataUnderSelectionRectangle(params.m_rectanglePos, camera, window, scene->m_res);
	
	//TODO Edge
	//TODO Vertex

	return !toolParams->m_selectedData.empty();
}

void SelectionLayerCallBack::execute(const SelectionLayerParams& params)
{
	m_selectionMode = ViewPortsHolderContext::s_selectionController->selectionMode();
	
	SelectionToolParams* toolParams = new SelectionToolParams();
	bool select{};
	
	if (isClick(params.m_rectanglePos))
	{
		if (ViewPortsHolderContext::s_uiLayerController->clickedOnUiWindow(params.m_rectanglePos.startPos))
		{
			return;
		}

		select = isSelectedByClick(params, toolParams);
	}
	else
	{
		select = isSelectedByRectangle(params, toolParams);
	}
	
	ITool* tool = nullptr;

	switch (m_selectionMode)
	{
		case SelectionMode::MESH:
			tool = select ? ToolRegistry::instance().getTool(MESH_SELECTION_TOOL)
						  : ToolRegistry::instance().getTool(MESH_DESELECTION_TOOL);
			break;
		case SelectionMode::FACE:
			// selectionTool = select ? ToolRegistry::instance().getTool(FACE_SELECTION_TOOL)
			// 					   : ToolRegistry::instance().getTool(FACE_DESELECTION_TOOL);
			//TODO
			break;
		case SelectionMode::EDGE:
			//TODO
			break;
		case SelectionMode::VERTEX:
			//TODO
			break;
		default:
			break;
	}

	if (tool == nullptr)
	{
		return;
	}
	ViewPortsHolderContext::s_viewPortsController->m_currentTool = tool;

	if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
	{
		delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
		ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
	}
	if (select)
	{
		ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = toolParams;
	}
	else
	{
		delete toolParams;
	}
}