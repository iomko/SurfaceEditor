#pragma once
#include "../Tools/ToolRegistry.h"
#include "../Tools/ToolIDs.h"

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
				auto* faceSelectionTool = ToolRegistry::instance().getTool(FACE_SELECTION_TOOL);
				if(faceSelectionTool)
				{
					ViewPortsHolderContext::s_viewPortsController->m_currentTool = faceSelectionTool;

					if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
					{
						delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
						ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
					}
				}				

			} else if(selectionMode == SelectionLayerParams::SelectionMode::Edge)
			{
				//ViewPortsHolderContext::s_viewPortsController->m_currentCommand = nullptr;
				//create edge command
			} else if(selectionMode == SelectionLayerParams::SelectionMode::Vertex)
			{
				//create vertex command
				//ViewPortsHolderContext::s_viewPortsController->m_currentCommand = nullptr;

			} else if(selectionMode == SelectionLayerParams::SelectionMode::Object)
			{
				auto* meshSelectionTool = ToolRegistry::instance().getTool(MESH_SELECTION_TOOL);
				if(meshSelectionTool)
				{
					ViewPortsHolderContext::s_viewPortsController->m_currentTool = meshSelectionTool;

					if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
					{
						delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
						ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
					}
				}
			} 
		} else if(type == SelectionLayerParams::Type::Deselection)
		{
			if (selectionMode == SelectionLayerParams::SelectionMode::Face)
			{
				auto* faceDeselectionTool = ToolRegistry::instance().getTool(FACE_DESELECTION_TOOL);
				if(faceDeselectionTool)
				{
					ViewPortsHolderContext::s_viewPortsController->m_currentTool = faceDeselectionTool;

					if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
					{
						delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
						ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
					}
				}
			}
			else if (selectionMode == SelectionLayerParams::SelectionMode::Edge)
			{
				//ViewPortsHolderContext::s_viewPortsController->m_currentCommand = nullptr;
				//create edge command
			}
			else if (selectionMode == SelectionLayerParams::SelectionMode::Vertex)
			{
				//ViewPortsHolderContext::s_viewPortsController->m_currentCommand = nullptr;
				//create vertex command	
			}
			else if (selectionMode == SelectionLayerParams::SelectionMode::Object)
			{
                
				auto* meshDeselectionTool = ToolRegistry::instance().getTool(MESH_DESELECTION_TOOL);
				if(meshDeselectionTool)
				{
					ViewPortsHolderContext::s_viewPortsController->m_currentTool = meshDeselectionTool;

					if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
					{
						delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
						ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
					}
				}
                
				//create object command
				//ViewPortsHolderContext::s_viewPortsController->m_currentCommand = CommandRegistry::getCommand<DeselectMeshCommand>();
			}

		}
	}
};
