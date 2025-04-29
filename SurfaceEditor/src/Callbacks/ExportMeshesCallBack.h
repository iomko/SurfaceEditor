#pragma once
#include "Callback.h"

#include "../ViewPortsHolder.h"
#include "../Commands/ImportMeshesCommand.h"
#include "../Commands/ExportMeshesCommand.h"

#include "../OBJExporter.h"
#include <random>


class ExportMeshesCallback : public Callback<ImportExportMeshesParams>, public Observer
{
public:
	void execute(const ImportExportMeshesParams& params) override
	{
		OBJExporter objExporter;
		bool fileOpened = false;
		objExporter.setFilePath(params.m_filePathMeshes);

		for (const auto selectedMesh : ViewPortsHolderContext::m_viewPortsHolder->m_selectedMeshes)
		{
			selectedMesh->m_meshID = selectedMesh->m_meshID;
			objExporter.parseMesh(selectedMesh);
		}

		if (ViewPortsHolderContext::m_viewPortsHolder->m_selectedMeshes.size() == 0)
		{
			if (std::filesystem::exists(params.m_filePathMeshes))
			{
				std::filesystem::remove(params.m_filePathMeshes);
			}
		}
		else
		{
			objExporter.write();
		}
	}
};

/*
class ExportMeshesCallable : public Callback, public Observer
{
public:
	virtual void execute(const Params& cmdParams) override
	{
		const ImportExportMeshesParams& castedCmdParams = static_cast<const ImportExportMeshesParams&>(cmdParams);

		OBJExporter objExporter;
		bool fileOpened = false;
		objExporter.setFilePath(castedCmdParams.m_filePathMeshes);

		for (const auto selectedMesh : ViewPortsHolderContext::m_viewPortsHolder->m_meshes)
		{
			selectedMesh->m_meshID = selectedMesh->m_meshID;
			objExporter.parseMesh(selectedMesh);
		}

		if (ViewPortsHolderContext::m_viewPortsHolder->m_meshes.size() == 0)
		{
			if (std::filesystem::exists(castedCmdParams.m_filePathMeshes))
			{
				std::filesystem::remove(castedCmdParams.m_filePathMeshes);
			}
		}
		else
		{
			objExporter.write();
		}
	}
};
*/