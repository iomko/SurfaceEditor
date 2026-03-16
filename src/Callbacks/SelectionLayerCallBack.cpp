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

void SelectionLayerCallBack::execute(const SelectionLayerParams& params)
{
	Camera* camera = ViewPortsHolderContext::s_camera;
    Window* window = ViewPortsHolderContext::s_window;
    Scene* scene   = ViewPortsHolderContext::s_viewPortsController->m_scene;

	SelectionToolParams* toolParams = new SelectionToolParams();
	toolParams->m_meshFaceHitPair 	= SceneUtilities::retClosestHitData(camera, window, scene->m_res);
	toolParams->m_isClick			= isClick(params.m_rectanglePos);

	auto mesh = toolParams->m_meshFaceHitPair.first.first;
	auto face = toolParams->m_meshFaceHitPair.first.second;

	bool select = mesh || face; // || edge || vertex - TODO

	ITool* tool = nullptr;

	switch (params.m_selectionMode)
	{
		case SelectionLayerParams::SelectionMode::Object:
			tool = select ? ToolRegistry::instance().getTool(MESH_SELECTION_TOOL)
						  : ToolRegistry::instance().getTool(MESH_DESELECTION_TOOL);
			break;
		case SelectionLayerParams::SelectionMode::Face:
			// selectionTool = select ? ToolRegistry::instance().getTool(FACE_SELECTION_TOOL)
			// 					   : ToolRegistry::instance().getTool(FACE_DESELECTION_TOOL);
			break;
		case SelectionLayerParams::SelectionMode::Edge:
			//TODO
			break;
		case SelectionLayerParams::SelectionMode::Vertex:
			//TODO
			break;
		default:
			break;
	}

	if (tool == nullptr)
	{
		return;
	}
	tool->getInteractionHandler()->onUpdate(*toolParams);

	ViewPortsHolderContext::s_viewPortsController->m_currentTool = tool;

	if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
	{
		delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
	}
	ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = toolParams;
}