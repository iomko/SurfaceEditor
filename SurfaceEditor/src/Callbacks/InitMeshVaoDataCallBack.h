#pragma once
#include "Callback.h"
#include "../Commands/CmdProperties/CmdProperties.h"
#include "../Mesh.h"

#include "../ViewPortsHolder.h"


class InitMeshVaoDataCallBack : public Callback
{
public:
	void execute(const Params& params) override
	{
		const MeshParams& initMeshVaoDataParams = static_cast<const MeshParams&>(params);

		Mesh* mesh = initMeshVaoDataParams.m_mesh;
		std::vector<HalfEdgeDS::Face>& meshFaces = initMeshVaoDataParams.m_mesh->getHalfEdgeStructure()->m_faces;

		Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;

		SceneRendererData::MeshVaoDataMap& meshVaoDataMap = scene->m_rendererData.meshData.meshVaoDataMap;
		SceneRendererData::MeshFacesMap& meshFacesMap = scene->m_rendererData.meshData.meshFacesMap;
		SceneRendererData::MeshMaterialsMap& meshMaterialsMap = scene->m_rendererData.meshData.meshMaterialsMap;


		//---FACEINFO MAPPING---
		SceneRendererData::MeshFacesMap::iterator meshFacesMapIt = meshFacesMap.emplace(mesh, SceneRendererData::FaceInfoMap()).first;
		SceneRendererData::FaceInfoMap& faceInfoMap = meshFacesMapIt->second; //I WILL BE USING JUST THIS MAP LATER

		//---MATERIALS MAPPING---
		SceneRendererData::MeshMaterialsMap::iterator meshMaterialsMapIt = meshMaterialsMap.emplace(mesh, SceneRendererData::MaterialFacesMap()).first;
		SceneRendererData::MaterialFacesMap& materialFacesMap = meshMaterialsMapIt->second;
		std::vector<HalfEdgeDS::FaceIndex>& materialFacesVector = materialFacesMap[mesh->m_defaultMaterial]; //I WILL BE USING JUST THIS VECTOR LATER

		//---VAO VERTICES MAPPING---
		SceneRendererData::MeshVaoDataMap::iterator meshVaoDataMapIt = meshVaoDataMap.emplace(mesh, SceneRendererData::MaterialVertexMap()).first;
		SceneRendererData::MaterialVertexMap& materialVertexMap = meshVaoDataMapIt->second;
		std::vector<MeshVertex>& materialVerticesVector = materialVertexMap[mesh->m_defaultMaterial]; //I WILL BE USING JUST THIS VECTOR LATER


		int meshFaceStartIndex = 0; //START INDEX OF FACE IN SPECIFIC MESH
		int verticesAmountInFace = 0;
		int faceVectorIndexInMaterial = 0; //INDEX IN STD::VECTOR<HALFEDGEDS::FACE*> FOR THE SPECIFIED MATERIAL*
		for (HalfEdgeDS::Face& meshFace : meshFaces)
		{
			verticesAmountInFace = 0;
			//TODO: NA SPOCITANIE POCTU VERTEXOV, TREBA UPRAVIT H HALFEDGE STRUKTURE NESKOR, NECH NEMUSIM TAKTO MANUALNE PRECHADZAT VERTICES
			for (auto it = meshFace.faceVertexBegin(); it != meshFace.faceVertexEnd(); ++it) {
				++verticesAmountInFace;
			}

			//---CREATION OF FACEINFO---
			SceneRendererData::FaceInfo faceInfo;

			faceInfo.normal = PolygonOperations::computeFaceNormal(&meshFace);
			faceInfo.startIndex = meshFaceStartIndex;
			faceInfo.endIndex = meshFaceStartIndex + (verticesAmountInFace -1); //-1 KVOLI SPRAVNEMU INDEXOVANIU
			faceInfo.material = mesh->m_defaultMaterial; //ZATIAL TAM PRIDAME DEFAULT MATERIAL
			faceInfo.indexInMaterial = faceVectorIndexInMaterial;

			HalfEdgeDS::FaceIndex faceIndex = meshFace.getHalfEdge().operator*().getFaceIndex();

			//---FACEINFO MAPPING---
			faceInfoMap[faceIndex] = faceInfo;
			
			//---MATERIALS MAPPING---
			materialFacesVector.emplace_back(faceIndex);

			//---VAO VERTICES MAPPING---
			computeVAOFaceData(&meshFace, materialVerticesVector);

			//UPDATE OF COUNTERS
			meshFaceStartIndex = meshFaceStartIndex + verticesAmountInFace;
			++faceVectorIndexInMaterial;
		}

	}

private:

	void computeVAOFaceData(HalfEdgeDS::Face* face, std::vector<MeshVertex>& facesVao)
	{
		glm::vec3 faceNormal = PolygonOperations::computeFaceNormal(face);
		//glm::vec3 faceNormal = computeFaceNormal(face);

		for (auto it = face->faceVertexBegin(); it != face->faceVertexEnd(); ++it) {
			HalfEdgeDS::Vertex vertex = it.operator*();
			facesVao.emplace_back(vertex.getPosition(), faceNormal, false);
		}
	}
};
