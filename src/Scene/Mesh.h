#pragma once
#include "../Renderer/Material.h"
#include "../Structures/ExtendedHalfEdge.h"
#include "Primitives/AABB.h"
#include <limits>
#include "../Renderer/BufferLayouts.h"
#include <glm/gtx/matrix_decompose.hpp>


class Mesh
{
public:
	ExtendedHalfEdgeMesh* m_halfEdgeStructure = nullptr;

	std::string m_meshID;
	
	bool m_selected = false;
	int m_selectionIndex = -1;

	bool m_buildSuccessful = false;
    
    AABBBoundingRegion m_meshBounds;

    MeshBufferLayout bufferLayout;

	glm::mat4 m_transform = glm::mat4(1.0f);
    glm::mat4* m_realTimeTransform = nullptr;

public:

	Mesh(std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices, const std::vector<int>& polygonNormalIndices, const std::vector<glm::vec3>& polygonsNormals = std::vector<glm::vec3>())
	{
		m_halfEdgeStructure = new ExtendedHalfEdgeMesh();
		m_halfEdgeStructure->build(polygonsIndices, polygonsVertices);
       
        calculateMeshBounds();
	}

	Mesh(std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices)
	{
		m_halfEdgeStructure = new ExtendedHalfEdgeMesh();
		m_halfEdgeStructure->build(polygonsIndices, polygonsVertices);

        calculateMeshBounds();
	}

    void calculateMeshBounds() {
        glm::vec3 minBounds(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        glm::vec3 maxBounds(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
        
        for(ExtendedEdge* edge : m_halfEdgeStructure->m_edges) {
            minBounds = glm::min(minBounds, edge->m_firstVertex->m_position);
            minBounds = glm::min(minBounds, edge->m_secondVertex->m_position);

            maxBounds = glm::max(maxBounds, edge->m_firstVertex->m_position);
            maxBounds = glm::max(maxBounds, edge->m_secondVertex->m_position);
        }

        m_meshBounds.setBounds(minBounds, maxBounds);
    }

	bool isBuildSuccessful()
	{
		return true;
		return m_buildSuccessful;
	}

	ExtendedHalfEdgeMesh* getHalfEdgeStructure()
	{
		return m_halfEdgeStructure;
	}

};
