#pragma once
#include "Callback.h"
#include "../ViewPortHolder.h"
#include "../Gui/ToolBarLayer.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/SelectMeshCommand.h"
#include "../Commands/DeselectMeshCommand.h"
#include "../Commands/SelectFaceCommand.h"
#include "../Commands/SelectVertexCommand.h"
#include "../Callbacks/VertexModeChangeCallBack.h"

class ToolBarLayerCallBack : public Callback
{
public:

	ToolBarLayerCallBack(ViewPortHolder* viewPortHolder, ToolBarLayer* toolBarLayer)
		: m_viewPortHolder(viewPortHolder),
		m_toolBarLayer(toolBarLayer),
		m_vertexModeChangeCallBack(viewPortHolder) {}

	void execute() override
	{
		std::cout << "Executed ToolBarLayerCallBack" << std::endl;
		ToolBarLayer::SelectionMode selectionMode = m_toolBarLayer->getSelectionMode();
		ToolBarLayer::Type type = m_toolBarLayer->getType();

		//potrebujeme teraz metodu

		if(type == ToolBarLayer::Selection)
		{
			if(selectionMode == ToolBarLayer::SelectionMode::Face)
			{
				//create face command
				m_viewPortHolder->m_viewPortLayer->m_currentSelectedCommand = CommandRegistry::getCommand<SelectFaceCommand>();
			} else if(selectionMode == ToolBarLayer::SelectionMode::Edge)
			{
				m_viewPortHolder->m_viewPortLayer->m_currentSelectedCommand = nullptr;
				//create edge command
			} else if(selectionMode == ToolBarLayer::SelectionMode::Vertex)
			{
				//create vertex command
				m_viewPortHolder->m_viewPortLayer->m_currentSelectedCommand = CommandRegistry::getCommand<SelectVertexCommand>();
				//callback VertexModeChangeCallBack
				m_vertexModeChangeCallBack.execute();

			} else if(selectionMode == ToolBarLayer::SelectionMode::Object)
			{
				//create object command
				m_viewPortHolder->m_viewPortLayer->m_currentSelectedCommand = CommandRegistry::getCommand<SelectMeshCommand>();
			}
		} else if(type == ToolBarLayer::Deselection)
		{
			if (selectionMode == ToolBarLayer::SelectionMode::Face)
			{
				m_viewPortHolder->m_viewPortLayer->m_currentSelectedCommand = nullptr;
				//create face command
			}
			else if (selectionMode == ToolBarLayer::SelectionMode::Edge)
			{
				m_viewPortHolder->m_viewPortLayer->m_currentSelectedCommand = nullptr;
				//create edge command
			}
			else if (selectionMode == ToolBarLayer::SelectionMode::Vertex)
			{
				m_viewPortHolder->m_viewPortLayer->m_currentSelectedCommand = nullptr;
				//create vertex command	
			}
			else if (selectionMode == ToolBarLayer::SelectionMode::Object)
			{
				//create object command
				m_viewPortHolder->m_viewPortLayer->m_currentSelectedCommand = CommandRegistry::getCommand<DeselectMeshCommand>();
			}

		}
	}
private:
	ViewPortHolder* m_viewPortHolder;
	ToolBarLayer* m_toolBarLayer;
	//internal callbacks
	VertexModeChangeCallBack m_vertexModeChangeCallBack;
};
