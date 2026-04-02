#include "ImportMeshesCallBack.h"
#include "../../src/ViewPortsController.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "OBJImporter.h"

#include "../../src/Callables/CallableRegistry.h"

static AutoRegisterCallback<ImportMeshesCallback> autoRegister("IMPORT_MESHES_CALLBACK");

void ImportMeshesCallback::execute(const ImportExportMeshesParams &params)
{
    OBJImporter objImporter;
    objImporter.setFilePath(params.m_filePathMeshes);
    objImporter.read();

    for (const auto &importedMesh : objImporter.getMeshes())
    {
        auto callable1 = CallableRegistry::instance().getCallable("MESH_VAO_INIT_CALLABLE");
        auto callable2 = CallableRegistry::instance().getCallable("SCENE_MESH_ADDER_CALLABLE");
        auto callable3 = CallableRegistry::instance().getCallable("MESH_OUTLINER_ADDER_CALLABLE");

        if(callable1 && callable2) 
        {
            MeshParams params;
            params.m_mesh = importedMesh;
            callable1->execute(params);
            callable2->execute(params);
            if(callable3)
            {
                callable3->execute(params);
            }
        }
    }
}
void ImportMeshesCallback::createMeshRenderingData(const ImportExportMeshesParams& cmdParams, Mesh* mesh){}
