#include "SelectionLayerCallBack.h"
#include "CallbackRegister.h"
#include "../Tools/ToolRegistry.h"
#include "../Tools/ToolIDs.h"
#include "../ViewPortsController.h"
#include "../CsvFeatureExporter.h"

static AutoRegisterCallback<SelectionLayerCallBack> autoRegisterSelectionLayerCallBack;

void SelectionLayerCallBack::execute(const SelectionLayerParams &params)
{
    SelectionLayerParams::SelectionMode selectionMode = params.m_selectionMode;
		SelectionLayerParams::Type type = params.m_type;

		if(type == SelectionLayerParams::Type::Selection)
		{
			if(selectionMode == SelectionLayerParams::SelectionMode::Face)
			{
				auto* faceSelectionTool = ToolRegistry::instance().getTool(FACE_SELECTION_TOOL);
                if(faceSelectionTool == nullptr)
                {
                    return;
                }
				ViewPortsHolderContext::s_viewPortsController->m_currentTool = faceSelectionTool;

				if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
				{
					delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
					ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
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
                if(meshSelectionTool == nullptr)
                {
                    return;
                }
				ViewPortsHolderContext::s_viewPortsController->m_currentTool = meshSelectionTool;

				if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
				{
					delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
					ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
				}
			}
		} else if(type == SelectionLayerParams::Type::Deselection)
		{
			if (selectionMode == SelectionLayerParams::SelectionMode::Face)
			{
				auto* faceDeselectionTool = ToolRegistry::instance().getTool(FACE_DESELECTION_TOOL);
                if(faceDeselectionTool == nullptr)
                {
                    return;
                }
				ViewPortsHolderContext::s_viewPortsController->m_currentTool = faceDeselectionTool;

				if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
				{
					delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
					ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
				}

			}
			else if (selectionMode == SelectionLayerParams::SelectionMode::Edge)
			{
				//ViewPortsHolderContext::s_viewPortsController->m_currentCommand = nullptr;

			}
			else if (selectionMode == SelectionLayerParams::SelectionMode::Vertex)
			{
				//ViewPortsHolderContext::s_viewPortsController->m_currentCommand = nullptr;
                
                /*
                CsvFeatureExporter csvFeatureExporter;
                Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

                for(auto& [mesh, _] : scene->m_res.meshFaceOctreeCoordsMap) {
                    csvFeatureExporter.appendToCsv(mesh); 
                } 
                */
                
			}
			else if (selectionMode == SelectionLayerParams::SelectionMode::Object)
			{
                
				auto* meshDeselectionTool = ToolRegistry::instance().getTool(MESH_DESELECTION_TOOL);
                if(meshDeselectionTool == nullptr)
                {
                    return;
                }
				ViewPortsHolderContext::s_viewPortsController->m_currentTool = meshDeselectionTool;

				if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
				{
					delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
					ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
				}
                
				//create object command
				//ViewPortsHolderContext::s_viewPortsController->m_currentCommand = CommandRegistry::getCommand<DeselectMeshCommand>();
			}

		}
}