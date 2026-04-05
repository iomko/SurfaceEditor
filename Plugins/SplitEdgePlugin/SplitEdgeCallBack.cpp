#include "SplitEdgeCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"

#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/ViewPortsController.h"
#include "../../src/Renderer/MaterialRegistry.h"

static AutoRegisterCallback<SplitEdgeCallBack> registerCallback("SPLIT_EDGE_CALLBACK");

SplitEdgeCallBack::SplitEdgeCallBack() {}

void SplitEdgeCallBack::execute(const SplitEdgeParams& params) {

    Mesh* mesh = params.mesh;
    ExtendedEdge* edge = params.edge;
    std::tuple<ExtendedEdge*, ExtendedEdge*, ExtendedEdge*, ExtendedEdge*> splitedEdges = mesh->m_halfEdgeStructure->splitEdge(edge);
    ExtendedEdge* firstSplitEdge = std::get<0>(splitedEdges);
    ExtendedEdge* secondSplitEdge = std::get<1>(splitedEdges);
    ExtendedEdge* thirdSplitEdge = std::get<2>(splitedEdges);
    ExtendedEdge* fourthSplitEdge = std::get<3>(splitedEdges);

    //update vao of existing faces
    //first Face
    updateFaceVao(mesh, firstSplitEdge->m_halfEdge->m_face);
    //second Face
    if (firstSplitEdge->m_halfEdge->m_twin != nullptr) {
        updateFaceVao(mesh, firstSplitEdge->m_halfEdge->m_twin->m_face);
    }

    //create vao of new faces
    //first Face
    createFaceVao(mesh, secondSplitEdge->m_halfEdge->m_face);
    //second Face
    if (secondSplitEdge->m_halfEdge->m_twin != nullptr) {
        createFaceVao(mesh, secondSplitEdge->m_halfEdge->m_twin->m_face);
    }

    //update vao of existing edge
    updateEdgeVao(mesh, firstSplitEdge);
    createEdgeVao(mesh, secondSplitEdge);
    createEdgeVao(mesh, thirdSplitEdge);
    if (fourthSplitEdge != nullptr) {
        createEdgeVao(mesh, fourthSplitEdge);
    }

    /*
    Material* defaultMeshMaterial = MaterialRegistry::getMaterial("defaultMeshMaterial");
    if(auto opt = mesh->bufferLayout.getTriangleBufferStorage(defaultMeshMaterial)) {
        TriangleBufferStorage& triangleBufferStorage = opt->get();
        triangleBufferStorage.update();
    }

    Material* defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");
    if(auto opt = mesh->bufferLayout.getLineBufferStorage(defaultLineMaterial)) {
        LineBufferStorage& lineBufferStorage = opt->get();
        lineBufferStorage.update();
    }
    */
}

void SplitEdgeCallBack::updateEdgeVao(Mesh *mesh, ExtendedEdge *edge) {

    Material* defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");

    if(auto opt = mesh->bufferLayout.getLineBufferStorage(defaultLineMaterial)) {
        LineBufferStorage& lineBufferStorage = opt->get();
        std::vector<BufferStorageDataType::LineVertex>& lineBufferVertices = lineBufferStorage.data.vertices;

        ExtendedVertex* firstVertex = edge->m_firstVertex;
        ExtendedVertex* secondVertex = edge->m_secondVertex;

        lineBufferVertices.at(edge->m_EdgeLineIndex).position = firstVertex->m_position;
        lineBufferVertices.at(edge->m_EdgeLineIndex + 1).position = secondVertex->m_position;
    }

}

void SplitEdgeCallBack::createEdgeVao(Mesh *mesh, ExtendedEdge *edge) {

    Material* defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");

    if(auto opt = mesh->bufferLayout.getLineBufferStorage(defaultLineMaterial)) {
        LineBufferStorage& lineBufferStorage = opt->get();
        std::vector<BufferStorageDataType::LineVertex>& lineBufferVertices = lineBufferStorage.data.vertices;

        ExtendedVertex* firstVertex = edge->m_firstVertex;
        ExtendedVertex* secondVertex = edge->m_secondVertex;

        ExtendedHalfEdge* halfEdge = edge->m_halfEdge;
        glm::vec3 normal = utils::geometry::computePolygonNormal(halfEdge->m_face);

        if (halfEdge->m_twin != nullptr)
        {
            glm::vec3 twinNormal = utils::geometry::computePolygonNormal(halfEdge->m_twin->m_face);
            normal = glm::normalize(normal + twinNormal);
        }

        lineBufferVertices.emplace_back(firstVertex->m_position, false);
        lineBufferVertices.emplace_back(secondVertex->m_position, false);

        int edgeLineIndex = lineBufferVertices.size() - 2;
        edge->m_EdgeLineIndex = edgeLineIndex;
    }

}

void SplitEdgeCallBack::updateFaceVao(Mesh *mesh, ExtendedFace *face) {

    Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

    std::vector<ExtendedVertex> faceVerts;
    ExtendedHalfEdgeMesh::getVerticesFromFace(face, faceVerts);

    FaceTriangleIndex faceTriangleIndex = face->faceTriangleIndices.front();
    FaceTriangle& faceTriangle = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second.at(faceTriangleIndex);
    Material* faceMaterial = MaterialRegistry::getMaterial(face->material->getName());

    if(auto opt = mesh->bufferLayout.getTriangleBufferStorage(faceMaterial)) {
        TriangleBufferStorage& triangleBufferStorage = opt->get();
        std::vector<BufferStorageDataType::TriangleVertex>& triangleBufferVertices = triangleBufferStorage.data.vertices;

        int faceIndexInVao = faceTriangle.indexInVAO;
        for (int i = faceIndexInVao; i < faceIndexInVao + 3; ++i)
        {
            triangleBufferVertices.at(i).position = faceVerts.at(i - faceIndexInVao).m_position;
        }

        scene->deleteFaceFromOctrees(mesh, face);
        scene->addFaceIntoOctrees(mesh, face);
    }

}

void SplitEdgeCallBack::createFaceVao(Mesh *mesh, ExtendedFace *face) {

    Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

    Material* defaultMeshMaterial = MaterialRegistry::getMaterial("defaultMeshMaterial");
    face->material = defaultMeshMaterial;
    glm::vec3 faceNormal = utils::geometry::computePolygonNormal(face);

    std::vector<FaceTriangle>& halfEdgeFaceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles[defaultMeshMaterial];

    //Triangulation
    std::vector<glm::vec3> faceVertices;
    for (auto it = face->faceVertexBegin(); it != face->faceVertexEnd(); ++it) {
        glm::vec3 vertexPosition = (*it).m_position;
        faceVertices.emplace_back(vertexPosition);
    }
    glm::vec3 firstVertex = faceVertices.at(0);
    faceVertices.emplace_back(firstVertex);

    std::vector<glm::vec3> triangulatedVertices = utils::geometry::triangulatePolygon(faceVertices);

    //Update ExtendedFace info
    Material* faceMaterial = MaterialRegistry::getMaterial(face->material->getName());
    if(auto opt = mesh->bufferLayout.getTriangleBufferStorage(faceMaterial)) {
        TriangleBufferStorage& triangleBufferStorage = opt->get();
        std::vector<BufferStorageDataType::TriangleVertex>& triangleBufferVertices = triangleBufferStorage.data.vertices;

        for (int i = 0, indexInFace = 0; i < triangulatedVertices.size(); i += 3, ++indexInFace)
        {
            triangleBufferVertices.emplace_back(triangulatedVertices.at(i), faceNormal, false, false);
            triangleBufferVertices.emplace_back(triangulatedVertices.at(i + 1), faceNormal, false, false);
            triangleBufferVertices.emplace_back(triangulatedVertices.at(i + 2), faceNormal, false, false);

            FaceTriangle faceTriangle;
            faceTriangle.face = face;
            faceTriangle.indexInVAO = triangleBufferVertices.size() - 3;
            faceTriangle.indexInFace = indexInFace;

            halfEdgeFaceTriangles.emplace_back(faceTriangle);

            int faceTriangleIndex = halfEdgeFaceTriangles.size() - 1;
            face->faceTriangleIndices.emplace_back(faceTriangleIndex);
        }

        scene->addFaceIntoOctrees(mesh, face);
    }

}
