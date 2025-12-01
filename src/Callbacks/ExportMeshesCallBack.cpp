#include "ExportMeshesCallBack.h"
#include "CallbackRegister.h"
#include "../IO/OBJExporter.h"

static AutoRegisterCallback<ExportMeshesCallback> registerExportMeshesCallback;
void ExportMeshesCallback::execute(const ImportExportMeshesParams &params)
{
    OBJExporter objExporter;
    bool fileOpened = false;
    objExporter.setFilePath(params.m_filePathMeshes);

    /*
    for (const auto selectedMesh : ViewPortsHolderContext::s_viewPortsController->m_selectedMeshes)
    {
        selectedMesh->m_meshID = selectedMesh->m_meshID;
        objExporter.parseMesh(selectedMesh);
    }

    if (ViewPortsHolderContext::s_viewPortsController->m_selectedMeshes.size() == 0)
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
    */
}