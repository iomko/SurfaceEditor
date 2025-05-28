#pragma once
#include "../Tools/FaceSelectionTool.h"
#include "../Tools/MeshSelectionTool.h"

class SelectionLayerCallBack : public Callback<SelectionLayerParams>, public Observer
{
public:
	virtual void execute(const SelectionLayerParams& params)
	{
		SelectionLayerParams::SelectionMode selectionMode = params.m_selectionMode;
		SelectionLayerParams::Type type = params.m_type;

		if(type == SelectionLayerParams::Type::Selection)
		{
			if(selectionMode == SelectionLayerParams::SelectionMode::Face)
			{
				FaceSelectionTool* faceSelectionTool = ToolRegistry::getTool<FaceSelectionTool>();
				ViewPortsHolderContext::s_viewPortsHolder->m_currentTool = faceSelectionTool;

				if (ViewPortsHolderContext::s_viewPortsHolder->m_currentToolParams != nullptr)
				{
					delete ViewPortsHolderContext::s_viewPortsHolder->m_currentToolParams;
					ViewPortsHolderContext::s_viewPortsHolder->m_currentToolParams = nullptr;
				}
			} else if(selectionMode == SelectionLayerParams::SelectionMode::Edge)
			{
				//ViewPortsHolderContext::s_viewPortsHolder->m_currentCommand = nullptr;
				//create edge command
			} else if(selectionMode == SelectionLayerParams::SelectionMode::Vertex)
			{
				//create vertex command
				//ViewPortsHolderContext::s_viewPortsHolder->m_currentCommand = nullptr;

			} else if(selectionMode == SelectionLayerParams::SelectionMode::Object)
			{
				MeshSelectionTool* meshSelectionTool = ToolRegistry::getTool<MeshSelectionTool>();
				ViewPortsHolderContext::s_viewPortsHolder->m_currentTool = meshSelectionTool;

				if (ViewPortsHolderContext::s_viewPortsHolder->m_currentToolParams != nullptr)
				{
					delete ViewPortsHolderContext::s_viewPortsHolder->m_currentToolParams;
					ViewPortsHolderContext::s_viewPortsHolder->m_currentToolParams = nullptr;
				}
			}
		} else if(type == SelectionLayerParams::Type::Deselection)
		{
			if (selectionMode == SelectionLayerParams::SelectionMode::Face)
			{
				//ViewPortsHolderContext::s_viewPortsHolder->m_currentCommand = CommandRegistry::getCommand<DeselectFaceCommand>();
				//create face command
			}
			else if (selectionMode == SelectionLayerParams::SelectionMode::Edge)
			{
				//ViewPortsHolderContext::s_viewPortsHolder->m_currentCommand = nullptr;
				//create edge command
			}
			else if (selectionMode == SelectionLayerParams::SelectionMode::Vertex)
			{
				//ViewPortsHolderContext::s_viewPortsHolder->m_currentCommand = nullptr;
				//create vertex command	
			}
			else if (selectionMode == SelectionLayerParams::SelectionMode::Object)
			{
				//create object command
				//ViewPortsHolderContext::s_viewPortsHolder->m_currentCommand = CommandRegistry::getCommand<DeselectMeshCommand>();
			}

		}
	}
};