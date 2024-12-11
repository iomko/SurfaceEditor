#pragma once
#include "Material.h"
#include "DataStructures/HalfEdge.h"
#include "Renderer/VertexDataStructs.h"

#include "PolygonOperations.h"
#include "VectorOperations.h"


//helper structs
struct Element
{
	Material* material;
	size_t index;
	bool deleted;
};

struct MovedElement
{
	Material* material;
	size_t index;
	size_t newIndexInMaterialVector;
};

struct StartEndOfElement
{
	size_t start;
	size_t end;
};

struct StartEndMovedElements
{
	std::vector<StartEndOfElement> startEndOfElements;
	std::vector<MovedElement> movedElements;
};
//

struct VectorData
{
	std::vector<Element> m_materialIDIndexMap;
	//prvy vektor indexom odkazuje na vektor pod nim
	std::vector<MeshVertex> m_vertexData;
};

struct FaceData
{
	size_t faceIndex;
	size_t startIndex;
	size_t endIndex;
	glm::vec3 normal;
};

#include "GeometryAlgorithms/TriangleTriangulator.h"
#include "GeometryAlgorithms/NgonPlanarTriangulator.h"


class Mesh
{
public:
	//chceme sem pridat mapovanie materialov k jednotlivym vaos
	HalfEdgeDS::HalfEdgeMesh* m_halfEdgeMesh = nullptr;
	//std::vector<TestFace*> m_selectedFaces;
	//std::vector<HalfEdgeDS::Face*> m_selectedFaces;

	//co musi mat octree, je samozrejme Face*, taktiez ale aj start a end indexy


	//MeshMaterialStructure
	std::map<Material*, VectorData> m_materialIDVertexDataMap;
	VectorData m_combinedVertexDataMatVector;


	Material* m_defaultMaterial;


	std::vector<FaceData> m_facesData;

	std::string m_meshID;

	//mesh by nemal priamo vediet o tychto triangulators, su to len triedy, ktore sa pouzivaju docasne. Neskor ich uz netreba
	PolygonTriangulator* m_polygonTriangulator;

	//MeshRenderingData m_meshRenderingData;

	bool m_buildSuccessful = false;
public:

	//nemusi brat defaultMaterial
	Mesh(Material* defaultMaterial, PolygonTriangulator* triangulator, std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices, const std::vector<int>& polygonNormalIndices, const std::vector<glm::vec3>& polygonsNormals = std::vector<glm::vec3>())
	{
		m_defaultMaterial = defaultMaterial;
		m_polygonTriangulator = triangulator;
		m_halfEdgeMesh = new HalfEdgeDS::HalfEdgeMesh();
		m_halfEdgeMesh->build(polygonsIndices, polygonsVertices);
		m_buildSuccessful = buildMeshData(polygonsIndices, polygonsVertices, polygonNormalIndices, polygonsNormals);
	}

	Mesh(Material* defaultMaterial, PolygonTriangulator* triangulator, std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices)
	{
		m_defaultMaterial = defaultMaterial;
		m_polygonTriangulator = triangulator;
		m_halfEdgeMesh = new HalfEdgeDS::HalfEdgeMesh();
		m_halfEdgeMesh->build(polygonsIndices, polygonsVertices);
		m_buildSuccessful = buildMeshData(polygonsIndices, polygonsVertices, std::vector<int>(), std::vector<glm::vec3>());
	}

	bool isBuildSuccessful()
	{
		return m_buildSuccessful;
	}

	//tato metoda trianguluje jednotlive faces a taktiez prideli jednotlivym faces materialy, Tak aby sme mohli tiež rychlo pristupit k jednotlivym faces daneho materialu v danom meshi
	bool buildMeshData(std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices, const std::vector<int>& polygonNormalIndices = std::vector<int>(), const std::vector<glm::vec3>& polygonsNormals = std::vector<glm::vec3>())
	{
		size_t startIndexOfTrianglesInFace = 0;
		size_t endIndexOfTrianglesInFace = 0;
		size_t vertexIndex = 0;
		size_t currentFaceIndex = 0;
		const auto& materialIDVertexDataIt = m_materialIDVertexDataMap.insert({ m_defaultMaterial, VectorData() });

		bool triangulationSuccess;
		for (const auto& polygonIndices : polygonsIndices)
		{
			std::vector<glm::vec3> triangulatedVerticesOfPolygon;
			glm::vec3 polygonNormal;

			if(!m_polygonTriangulator->calculateTriangulation(polygonIndices, polygonsVertices, triangulatedVerticesOfPolygon, polygonNormal))
			{
				triangulationSuccess = false;
			} else
			{
				triangulationSuccess = true;
			}

			if(triangulationSuccess)
			{
				if (!polygonsNormals.empty() && !polygonNormalIndices.empty()) {

					polygonNormal = polygonsNormals.at(polygonNormalIndices.at(currentFaceIndex));
				}

				// Update end index for the next iteration
				endIndexOfTrianglesInFace = startIndexOfTrianglesInFace + triangulatedVerticesOfPolygon.size() - 1;

				//mame k dispozicii triangulatedVertices daneho facu
				for (const auto& triangulatedVertexOfPolygon : triangulatedVerticesOfPolygon)
				{
					setInitialDataForVertex(m_combinedVertexDataMatVector, materialIDVertexDataIt.first->second, { m_defaultMaterial, vertexIndex, false }, triangulatedVertexOfPolygon, polygonNormal);
					++vertexIndex;
				}
				m_facesData.push_back({ currentFaceIndex, startIndexOfTrianglesInFace, endIndexOfTrianglesInFace, polygonNormal });
				++currentFaceIndex;
				// Update start index to start from the next index
				startIndexOfTrianglesInFace = endIndexOfTrianglesInFace + 1;
			} else
			{
				//triangulacia nebola uspesna
				return triangulationSuccess;
			}

		}
		return triangulationSuccess;
	}

	void setInitialDataForVertex(VectorData& combinedVertexDataMat, VectorData& materialVertexDataMat, const Element& elementData, const glm::vec3& vertexAfterTriangulation, const glm::vec3& polygonNormal)
	{
		combinedVertexDataMat.m_materialIDIndexMap.push_back({ elementData.material, elementData.index, elementData.deleted });
		combinedVertexDataMat.m_vertexData.push_back({ vertexAfterTriangulation, polygonNormal});

		materialVertexDataMat.m_materialIDIndexMap.push_back({ elementData.material, elementData.index, elementData.deleted });
		materialVertexDataMat.m_vertexData.push_back({ vertexAfterTriangulation, polygonNormal });
	}

	HalfEdgeDS::HalfEdgeMesh* getMeshData()
	{
		return m_halfEdgeMesh;
	}

private:

	//helper pri assignovani materialu (teda pri vymazavani materialu
	void assignMovedElementsOfDeletedFaces(const VectorData& currentVectorData,const std::vector<StartEndOfElement>& startAndEndOfDeletedFaces, std::vector<MovedElement>& movedElements)
	{
		size_t moveElementByAmount = 0;
		//teraz si zistim vsetky movedElementy a pridam ich do movedElements
		for (int j = 0; j < startAndEndOfDeletedFaces.size(); ++j)
		{
			int startIndex;
			int endIndex;
			//prveho end
			startIndex = startAndEndOfDeletedFaces.at(j).end + 1;

			if (startIndex != currentVectorData.m_materialIDIndexMap.size())
			{

				//druheho start
				if ((j + 1) < startAndEndOfDeletedFaces.size())
				{
					endIndex = startAndEndOfDeletedFaces.at(j + 1).start - 1;
				}
				else
				{
					//ak nie je uz startAndEndOfDeletedFaces tak musime sa dostat az po konec combinedVektora
					endIndex = currentVectorData.m_materialIDIndexMap.size() - 1;
				}

				//musime skontrolovat ci druha faca nenasleduje okamzite za prvou
				//v takomto pripade nemame ako vytvorit movedElement

				moveElementByAmount += (startAndEndOfDeletedFaces.at(j).end - startAndEndOfDeletedFaces.at(j).start) + 1;

				//pridat MovedElements do vektora
				for (size_t k = startIndex; k <= endIndex; ++k)
				{
					movedElements.push_back({currentVectorData.m_materialIDIndexMap.at(k).material, currentVectorData.m_materialIDIndexMap.at(k).index, k - moveElementByAmount});
				}

			}
		}
	}
};
