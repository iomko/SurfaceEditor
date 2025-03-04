#pragma once
#include "Material.h"
#include "DataStructures/HalfEdge.h"
#include "Renderer/VertexDataStructs.h"

#include "PolygonOperations.h"
#include "VectorOperations.h"

#include "GeometryAlgorithms/TriangleTriangulator.h"
#include "GeometryAlgorithms/NgonPlanarTriangulator.h"

class Mesh
{
public:
	HalfEdgeDS::HalfEdgeMesh* m_halfEdgeStructure = nullptr;
	Material* m_defaultMaterial;

	std::string m_meshID;

	//mesh by nemal priamo vediet o tychto triangulators, su to len triedy, ktore sa pouzivaju docasne. Neskor ich uz netreba
	PolygonTriangulator* m_polygonTriangulator;

	bool m_buildSuccessful = false;
public:

	//nemusi brat defaultMaterial
	Mesh(Material* defaultMaterial, PolygonTriangulator* triangulator, std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices, const std::vector<int>& polygonNormalIndices, const std::vector<glm::vec3>& polygonsNormals = std::vector<glm::vec3>())
	{

		m_defaultMaterial = defaultMaterial;
		m_polygonTriangulator = triangulator;
		m_halfEdgeStructure = new HalfEdgeDS::HalfEdgeMesh();
		m_halfEdgeStructure->build(polygonsIndices, polygonsVertices);
		//m_buildSuccessful = buildMeshData(polygonsIndices, polygonsVertices, polygonNormalIndices, polygonsNormals);
	}

	Mesh(Material* defaultMaterial, PolygonTriangulator* triangulator, std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices)
	{
		m_defaultMaterial = defaultMaterial;
		m_polygonTriangulator = triangulator;
		m_halfEdgeStructure = new HalfEdgeDS::HalfEdgeMesh();
		m_halfEdgeStructure->build(polygonsIndices, polygonsVertices);
		
		//call CallBack which will have the implementation where it will initialize the VAO data instead
		//so we no longer need to have buildMeshData method here.
		//m_buildSuccessful = buildMeshData(polygonsIndices, polygonsVertices, std::vector<int>(), std::vector<glm::vec3>());
	}

	bool isBuildSuccessful()
	{
		return m_buildSuccessful;
	}

	HalfEdgeDS::HalfEdgeMesh* getHalfEdgeStructure()
	{
		return m_halfEdgeStructure;
	}

};
