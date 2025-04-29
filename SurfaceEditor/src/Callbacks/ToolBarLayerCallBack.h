#pragma once
#include "Callback.h"
#include "../ViewPortsHolder.h"
#include "../Gui/ToolBarLayer.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/SelectMeshCommand.h"
#include "../Commands/DeselectMeshCommand.h"
#include "../Commands/SelectFaceCommand.h"
#include "../Commands/SelectVertexCommand.h"
#include "../ViewPortsHolder.h"
#include "../Commands/DeselectFaceCommand.h"

class ToolBarLayerCallBack : public Callback<ToolBarParams>, public Observer
{
public:
	virtual void execute(const ToolBarParams& params)
	{
		ToolBarParams::SelectionMode selectionMode = params.m_selectionMode;
		ToolBarParams::Type type = params.m_type;

		if(type == ToolBarParams::Type::Selection)
		{
			if(selectionMode == ToolBarParams::SelectionMode::Face)
			{
				//create face command
				ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand = CommandRegistry::getCommand<SelectFaceCommand>();
			} else if(selectionMode == ToolBarParams::SelectionMode::Edge)
			{
				ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand = nullptr;
				//create edge command
			} else if(selectionMode == ToolBarParams::SelectionMode::Vertex)
			{
				//create vertex command
				ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand = nullptr;

			} else if(selectionMode == ToolBarParams::SelectionMode::Object)
			{
				//create object command
				ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand = CommandRegistry::getCommand<SelectMeshCommand>();
			}
		} else if(type == ToolBarParams::Type::Deselection)
		{
			if (selectionMode == ToolBarParams::SelectionMode::Face)
			{
				ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand = CommandRegistry::getCommand<DeselectFaceCommand>();
				//create face command
			}
			else if (selectionMode == ToolBarParams::SelectionMode::Edge)
			{
				ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand = nullptr;
				//create edge command
			}
			else if (selectionMode == ToolBarParams::SelectionMode::Vertex)
			{
				ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand = nullptr;
				//create vertex command	
			}
			else if (selectionMode == ToolBarParams::SelectionMode::Object)
			{
				//create object command
				ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand = CommandRegistry::getCommand<DeselectMeshCommand>();
			}

		}
	}
};