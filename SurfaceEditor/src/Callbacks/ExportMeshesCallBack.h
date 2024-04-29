#pragma once
#include "Callback.h"

#include "../ViewPortHolder.h"
#include "../Commands/ImportMeshesCommand.h"
#include "../Commands/ExportMeshesCommand.h"

#include "../OBJExporter.h"
#include <random>

class ExportMeshesCallBack : public Callback, public Observable
{
public:
	ExportMeshesCallBack(ViewPortHolder* viewPortHolder, ExportMeshesCommand* exportMeshesCommand)
	{
		m_viewPortHolder = viewPortHolder;
		m_exportMeshesCommand = exportMeshesCommand;
	}

	virtual void execute() override
	{
		OBJExporter objExporter;
		bool fileOpened = false;
		fileOpened = objExporter.open(m_exportMeshesCommand->m_filePathMeshes);
		if(fileOpened)
		{
			int currentMeshIndex = 0;
			for (const auto selectedMesh : m_viewPortHolder->m_selectedMeshes)
			{
				selectedMesh->m_meshID = std::to_string(currentMeshIndex);
				objExporter.setMesh(selectedMesh);
				++currentMeshIndex;
			}

			if (m_viewPortHolder->m_selectedMeshes.size() == 0)
			{
				if (std::filesystem::exists(m_exportMeshesCommand->m_filePathMeshes))
				{
					std::filesystem::remove(m_exportMeshesCommand->m_filePathMeshes);
				}
			}
			else
			{
				objExporter.write();
			}
		}
	}
private:
	ViewPortHolder* m_viewPortHolder;
	ExportMeshesCommand* m_exportMeshesCommand;
};
