#include "ExportMeshesCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "OBJExporter.h"
#include "../../src/ViewPortsController.h"

static AutoRegisterCallback<ExportMeshesCallback> registerExportMeshesCallback("EXPORT_MESHES_CALLBACK");
void ExportMeshesCallback::execute(const ImportExportMeshesParams &params)
{
    OBJExporter objExporter;
    objExporter.setFilePath(params.m_filePathMeshes);
    
    if(params.exportOnlySelected)
    {
        for (const auto selectedMesh : ViewPortsHolderContext::s_selectionController->getHolder().meshes)
        {
            selectedMesh->m_meshID = selectedMesh->m_meshID;
            objExporter.parseMesh(selectedMesh);
        }
    }
    else
    {
        Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
        for (const auto& [mesh, _] : scene->m_res.meshFaceOctreeCoordsMap)
        {
            objExporter.parseMesh(mesh);
        }
    }

    if (ViewPortsHolderContext::s_selectionController->getHolder().meshes.size() == 0)
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