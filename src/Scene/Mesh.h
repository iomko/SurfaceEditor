#pragma once
#include "../Renderer/Material.h"
#include "../DataStructures/ExtendedHalfEdge.h"

class Mesh
{
public:
	ExtendedHalfEdgeMesh* m_halfEdgeStructure = nullptr;
	Material* m_defaultMaterial;

	std::string m_meshID;
	
	bool m_selected = false;
	int m_selectionIndex = -1;

	bool m_buildSuccessful = false;
public:

	//nemusi brat defaultMaterial
	Mesh(Material* defaultMaterial, std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices, const std::vector<int>& polygonNormalIndices, const std::vector<glm::vec3>& polygonsNormals = std::vector<glm::vec3>())
	{
		m_defaultMaterial = defaultMaterial;
		m_halfEdgeStructure = new ExtendedHalfEdgeMesh();
		m_halfEdgeStructure->build(polygonsIndices, polygonsVertices);
	}

	Mesh(Material* defaultMaterial, std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices)
	{
		m_defaultMaterial = defaultMaterial;
		m_halfEdgeStructure = new ExtendedHalfEdgeMesh();
		m_halfEdgeStructure->build(polygonsIndices, polygonsVertices);
	}

	bool isBuildSuccessful()
	{
		return m_buildSuccessful;
	}

	ExtendedHalfEdgeMesh* getHalfEdgeStructure()
	{
		return m_halfEdgeStructure;
	}

};
