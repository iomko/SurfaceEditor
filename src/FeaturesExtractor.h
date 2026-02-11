#pragma once
#include <algorithm>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <tuple>
#include "Scene/Mesh.h"
//#include "Structures/ExtendedHalfEdge.h"
#include <array>

class FeaturesExtractor
{
public:
    static std::array<float, 3> sideLengths2(ExtendedFace* face) {

        std::array<float, 3> sideLengths;
        
        ExtendedHalfEdge* currentHalfEdge = face->m_halfEdge;
        int i = 0;

        do {
            
            ExtendedEdge* edge = currentHalfEdge->m_edge;
            glm::vec3 v1Pos = edge->m_firstVertex->m_position;
            glm::vec3 v2Pos = edge->m_secondVertex->m_position;
            
            float length = glm::length(v1Pos - v2Pos);

            sideLengths[i] = length;
            
            currentHalfEdge = currentHalfEdge->m_next;
            ++i;
        } while(currentHalfEdge != face->m_halfEdge);

        return sideLengths;
    }

    static float ratio2(ExtendedFace* face) {
        std::array<float, 3> sideLengths = FeaturesExtractor::sideLengths2(face);
        
        float minLength = std::min({sideLengths[0], sideLengths[1], sideLengths[2]});
        float maxLength = std::max({sideLengths[0], sideLengths[1], sideLengths[2]});

        return maxLength/minLength;
    }

    static std::array<float, 3> internalAngles2(ExtendedFace* face) {

        std::array<float, 3> angles;
        ExtendedHalfEdge* currentHalfEdge = face->m_halfEdge;
        int i = 0;

        do {

            ExtendedVertex* prevVertex = currentHalfEdge->m_previous->m_vertex;
            ExtendedVertex* curVertex  = currentHalfEdge->m_vertex;
            ExtendedVertex* nextVertex = currentHalfEdge->m_next->m_vertex;

            glm::vec3 v1Pos = nextVertex->m_position - curVertex->m_position;
            glm::vec3 v2Pos = prevVertex->m_position - curVertex->m_position;

            float dotProduct = glm::dot(glm::normalize(v1Pos), glm::normalize(v2Pos));
            dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);

            float angleRadians = glm::acos(dotProduct);
            float angleDegrees = glm::degrees(angleRadians);

            angles[i] = angleDegrees;

            currentHalfEdge = currentHalfEdge->m_next;
            ++i;

        } while (currentHalfEdge != face->m_halfEdge);

        return angles;
    }

    static float area2(ExtendedFace* face) {
        
        ExtendedHalfEdge* currentHalfEdge = face->m_halfEdge;
        
        ExtendedVertex* prevVertex = currentHalfEdge->m_previous->m_vertex;
        ExtendedVertex* curVertex  = currentHalfEdge->m_vertex;
        ExtendedVertex* nextVertex = currentHalfEdge->m_next->m_vertex;

        glm::vec3 v1 = prevVertex->m_position - curVertex->m_position;
        glm::vec3 v2 = nextVertex->m_position - curVertex->m_position;

        glm::vec3 crossProduct = glm::cross(v1, v2);

        float area = 0.5f * glm::length(crossProduct);
        return area;
    }

    static std::array<float, 3> heights2(ExtendedFace* face) {
        
        std::array<float, 3> heights;

        std::array<float, 3> sideLengths = FeaturesExtractor::sideLengths2(face);

        float area = FeaturesExtractor::area2(face);
        heights[0] = (2.0f * area) / sideLengths[0];
        heights[1] = (2.0f * area) / sideLengths[1];
        heights[2] = (2.0f * area) / sideLengths[2];

        return heights;
    }

    static bool isSkewed2(ExtendedFace* face) {

        std::array<float, 3> angles = FeaturesExtractor::internalAngles2(face);
        float minAngle = std::min({angles[0], angles[1], angles[2]});
        float maxAngle = std::max({angles[0], angles[1], angles[2]});
        float ratio = FeaturesExtractor::ratio2(face);

        return ((minAngle < 10.0f) || (maxAngle > 150.0f) || ratio > 10.0f);
    }
};
