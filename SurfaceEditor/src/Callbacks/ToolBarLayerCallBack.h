#pragma once
#include "Callback.h"
#include "../ViewPortsHolder.h"
#include "../Gui/ToolBarLayer.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/SelectMeshCommand.h"
#include "../Commands/DeselectMeshCommand.h"
#include "../Commands/SelectFaceCommand.h"
#include "../Commands/SelectVertexCommand.h"
#include "../Callbacks/VertexModeChangeCallBack.h"
#include "../ViewPortsHolder.h"

class ToolBarLayerCallBack : public Callback, public Observer
{
public:

	virtual void execute(const Params& cmdParams)
	{
		const ToolBarParams& castedCmdParams = static_cast<const ToolBarParams&>(cmdParams);

		std::cout << "Executed ToolBarLayerCallBack" << std::endl;

		ToolBarParams::SelectionMode selectionMode = castedCmdParams.m_selectionMode;
		ToolBarParams::Type type = castedCmdParams.m_type;

		//potrebujeme teraz metodu

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
				ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand = CommandRegistry::getCommand<SelectVertexCommand>();
				//callback VertexModeChangeCallBack
				VertexModeChangeCallBack vertexModeChangeCallBack;
				vertexModeChangeCallBack.execute();

			} else if(selectionMode == ToolBarParams::SelectionMode::Object)
			{
				//create object command
				ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand = CommandRegistry::getCommand<SelectMeshCommand>();
			}
		} else if(type == ToolBarParams::Type::Deselection)
		{
			if (selectionMode == ToolBarParams::SelectionMode::Face)
			{
				ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand = nullptr;
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