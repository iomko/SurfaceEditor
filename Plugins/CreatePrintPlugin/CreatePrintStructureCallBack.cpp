#include "CreatePrintStructureCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"

#include "../../src/Structures/PrintableMesh.h"
#include "../../src/Renderer/Renderer.h"
#include "../../src/Scene/Scene.h"
#include "../../src/Utils/GeometryUtils.h"
#include "../../src/Utils/glm_overrides.h"
#include "../../src/ViewPortsController.h"
#include "../../src/Renderer/MaterialRegistry.h"

static AutoRegisterCallback<CreatePrintStructureCallBack> autoRegisterCreatePrintStructureCallBack("CREATE_PRINT_STRUCTURE_CALLBACK");

void CreatePrintStructureCallBack::execute(const PrintMeshSettingsParams &iParams)
{

    // Get or create Printable Mesh From SceneResources
    float perimeterHeight = iParams.height;
    Mesh *inputMesh = iParams.mesh;
    ViewPortsController *controller = ViewPortsHolderContext::s_viewPortsController.get();
    Scene *scene = controller->m_scene;

    auto [printableMeshIt, _] = scene->m_res.printableMeshMap.try_emplace(inputMesh, new PrintableMesh());
    PrintableMesh *outputPrintableMesh = printableMeshIt->second;

    // Go through all plane slices of the mesh
    float minPlaneY = inputMesh->m_meshBounds.getMinBoundsPos().y;
    float maxPlaneY = inputMesh->m_meshBounds.getMaxBoundsPos().y;
    minPlaneY = (std::trunc(minPlaneY * 10.0f) / 10.0f) + 0.1f;

    Plane plane;
    plane.point = glm::vec3(0.0f, minPlaneY, 0.0f);
    plane.normal = glm::vec3(0.0f, 1.0f, 0.0f);

    for (float planeHeight = plane.point.y; plane.point.y < maxPlaneY; plane.point.y += perimeterHeight)
    {
        // vytvor plane
        std::map<std::pair<glm::vec3, glm::vec3>, std::pair<ExtrudeEdge, ExtendedFace *>, glm::Vec3PairApproxComparator> printableEdgesMap;

        const glm::vec3 meshMin = inputMesh->m_meshBounds.getMinBoundsPos();
        const glm::vec3 meshMax = inputMesh->m_meshBounds.getMaxBoundsPos();
        const AABBBoundingRegion sliceBounds(
            glm::vec3(meshMin.x, plane.point.y, meshMin.z),
            glm::vec3(meshMax.x, plane.point.y, meshMax.z));

        std::vector<SceneResources::MeshFacePair> candidateFaces;
        scene->query(sliceBounds, candidateFaces);

        for (const SceneResources::MeshFacePair &data : candidateFaces)
        {
            auto &[mesh, face] = data;
            if (inputMesh == mesh)
            {
                // Try plane-face intersection
                utils::geometry::PlaneFaceIntersection planeIntersection = utils::geometry::planeIntersectsFace(plane, face);

                // If line intersected at two points of a triangle, then add that to the printable edges
                if (planeIntersection.type == utils::geometry::PlaneFaceIntersection::Type::Edge)
                {
                    glm::vec3 modifiedFirstPoint{planeIntersection.firstPoint.x, plane.point.y, planeIntersection.firstPoint.z};
                    glm::vec3 modifiedSecondPoint{planeIntersection.secondPoint.x, plane.point.y, planeIntersection.secondPoint.z};

                    printableEdgesMap.insert({{modifiedFirstPoint, modifiedSecondPoint},
                                              {ExtrudeEdge{modifiedFirstPoint, modifiedSecondPoint}, face}});
                }
            }
        }

        // Convert PrintableEdgesMap into a std::vector type
        std::vector<std::pair<ExtrudeEdge, ExtendedFace *>> printableEdges;
        printableEdges.reserve(printableEdgesMap.size());

        for (auto &[_, value] : printableEdgesMap)
        {
            printableEdges.emplace_back(value);
        }

        outputPrintableMesh->addPerimeterLayerLevel(inputMesh, printableEdges);
    }

    // Register Printable Mesh BufferStorage
    Material *defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");
    LineBufferStorage &lineBufferStorage = outputPrintableMesh->bufferLayout.registerLineBufferStorage(defaultLineMaterial);
    lineBufferStorage.create();

    // Get Raw Mesh Buffer Data For Specified Mesh
    std::vector<BufferStorageDataType::LineVertex> &lineBufferVertices = lineBufferStorage.data.vertices;
    lineBufferVertices.clear();

    // Update PrintableMesh info
    for (PrintLayerLevel &printLayerLevel : outputPrintableMesh->m_structure.printLayers)
    {
        std::vector<PerimeterOutline> &perimeterOutlines = printLayerLevel.perimeterOutlines;
        for (PerimeterOutline &perimeterOutline : perimeterOutlines)
        {

            for (auto it = perimeterOutline.points.begin(); it != perimeterOutline.points.end(); ++it)
            {

                if (perimeterOutline.filled)
                {
                    lineBufferVertices.emplace_back(it->first.firstPoint, true);
                    lineBufferVertices.emplace_back(it->first.secondPoint, true);
                }
                else
                {
                    lineBufferVertices.emplace_back(it->first.firstPoint, 0.25f);
                    lineBufferVertices.emplace_back(it->first.secondPoint, 0.25f);
                }
            }
        }

        std::vector<InfillLine> &infillLines = printLayerLevel.infillLines;

        for (InfillLine &infillLine : infillLines)
        {
            for (auto it = infillLine.points.begin(); it != infillLine.points.end(); std::advance(it, 2))
            {

                lineBufferVertices.emplace_back(it->pos, 0.5f);
                lineBufferVertices.emplace_back(std::next(it)->pos, 0.5f);
            }
        }
    }

    lineBufferStorage.update();

    // Add Printable Mesh into the scene
    SceneResources::PrintableMeshMap &printableMeshesMap = scene->m_res.printableMeshMap;
    auto [it, inserted] = printableMeshesMap.emplace(inputMesh, outputPrintableMesh);


    auto* addChildCallback = CallbackRegistry::instance().getCallback(TemplateOutlinerNodeAdderCallbackIDManger::instance().GetIndex<PrintableMesh>( true));
    Observable* observable = dynamic_cast<Observable*>(addChildCallback);
    if(observable)
    {
        AddChildOutlinerNodeCallBackParams<PrintableMesh> params;
        params.id = 1;
        params.name = "PrintableMesh1";
        params.data = outputPrintableMesh;
        
        observable->notifyObservers(params);
    }
}
