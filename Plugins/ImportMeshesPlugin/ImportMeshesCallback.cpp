#include "ImportMeshesCallBack.h"
#include "../../src/ViewPortsController.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "OBJImporter.h"

#include "../../src/Callables/CallableRegistry.h"
#include <filesystem>

static AutoRegisterCallback<ImportMeshesCallback> autoRegister("IMPORT_MESHES_CALLBACK");

void ImportMeshesCallback::execute(const ImportExportMeshesParams &params)
{
    OBJImporter objImporter;
    objImporter.setFilePath(params.m_filePathMeshes);
    if (!objImporter.read())
    {
        for (Mesh* mesh : objImporter.getMeshes())
        {
            delete mesh;
        }
        return;
    }

    const std::vector<Mesh*>& meshes = objImporter.getMeshes();
    auto* initializeMesh = CallableRegistry::instance().getCallable("MESH_VAO_INIT_CALLABLE");
    auto* addMeshToScene = CallableRegistry::instance().getCallable("SCENE_MESH_ADDER_CALLABLE");
    auto* addMeshToOutliner = CallableRegistry::instance().getCallable("MESH_OUTLINER_ADDER_CALLABLE");
    if (initializeMesh == nullptr || addMeshToScene == nullptr)
    {
        for (Mesh* mesh : meshes)
        {
            delete mesh;
        }
        throw std::logic_error("Mesh initialization and scene-adder callables are required for import.");
    }

    ViewPortsController* controller = ViewPortsHolderContext::s_viewPortsController.get();
    
    const std::string objectName = std::filesystem::path(params.m_filePathMeshes).stem().string();// Use the file name (without extension) as the object name
    Object* object = controller->createObject(objectName.empty() ? "Imported Object" : objectName);
    for (Mesh* mesh : meshes)
    {
        object->addMesh(mesh);
        mesh->m_parentObject = object;
    }

    for (size_t meshIndex = 0; meshIndex < meshes.size(); ++meshIndex)
    {
        Mesh* importedMesh = meshes[meshIndex];
        MeshParams meshParams;
        meshParams.m_mesh = importedMesh;
        meshParams.m_object = object;
        meshParams.m_name = "Mesh " + std::to_string(meshIndex + 1);
        initializeMesh->execute(meshParams);
        addMeshToScene->execute(meshParams);
        if (addMeshToOutliner != nullptr)
        {
            addMeshToOutliner->execute(meshParams);
        }
    }
}
void ImportMeshesCallback::createMeshRenderingData(const ImportExportMeshesParams& cmdParams, Mesh* mesh){}
