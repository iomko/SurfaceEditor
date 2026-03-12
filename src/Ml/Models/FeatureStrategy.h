#pragma once
#include "FeaturesExtractor.h"
#include "vector"
#include "Structures/ExtendedHalfEdge.h"

template<typename Type>
class FeatureStrategy {
public:
    virtual ~FeatureStrategy() = default;
    virtual std::vector<float> invoke(Type obj) = 0;
    virtual size_t getSize() = 0;
};

class FaceSideLengthsFeature : public FeatureStrategy<ExtendedFace*> {
public:
    std::vector<float> invoke(ExtendedFace* obj) override {
        std::vector<float> res;

        ExtendedHalfEdge* currentHalfEdge = obj->m_halfEdge;
        do {
            ExtendedEdge* edge = currentHalfEdge->m_edge;
            glm::vec3 v1Pos = edge->m_firstVertex->m_position;
            glm::vec3 v2Pos = edge->m_secondVertex->m_position;

            float length = glm::length(v1Pos - v2Pos);
            res.emplace_back(length);

            currentHalfEdge = currentHalfEdge->m_next;
        } while(currentHalfEdge != obj->m_halfEdge);

        return res;
    }

    size_t getSize() override {
        return 3;
    }
};

class FaceSideLengthRatioFeature : public FeatureStrategy<ExtendedFace*> {
public:
    std::vector<float> invoke(ExtendedFace* obj) override {

        std::vector<float> sideLengths;

        ExtendedHalfEdge* currentHalfEdge = obj->m_halfEdge;
        do {
            ExtendedEdge* edge = currentHalfEdge->m_edge;
            glm::vec3 v1Pos = edge->m_firstVertex->m_position;
            glm::vec3 v2Pos = edge->m_secondVertex->m_position;

            float length = glm::length(v1Pos - v2Pos);
            sideLengths.emplace_back(length);

            currentHalfEdge = currentHalfEdge->m_next;
        } while(currentHalfEdge != obj->m_halfEdge);

        float minLength = *std::min(sideLengths.begin(), sideLengths.end());
        float maxLength = *std::max(sideLengths.begin(), sideLengths.end());

        std::vector<float> res;
        res.emplace_back(maxLength/minLength);
        return res;
    }

    size_t getSize() override {
        return 1;
    }
};

class FaceInternalAnglesFeature : public FeatureStrategy<ExtendedFace*> {
public:
    std::vector<float> invoke(ExtendedFace* obj) override {
        std::vector<float> res;

        ExtendedHalfEdge* currentHalfEdge = obj->m_halfEdge;
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

            res.emplace_back(angleDegrees);

            currentHalfEdge = currentHalfEdge->m_next;
        } while (currentHalfEdge != obj->m_halfEdge);

        return res;
    }

    size_t getSize() override {
        return 3;
    }
};

class FaceAreaFeature : public FeatureStrategy<ExtendedFace*> {
public:
    std::vector<float> invoke(ExtendedFace* obj) override {
        std::vector<float> res;

        ExtendedHalfEdge* currentHalfEdge = obj->m_halfEdge;

        ExtendedVertex* prevVertex = currentHalfEdge->m_previous->m_vertex;
        ExtendedVertex* curVertex  = currentHalfEdge->m_vertex;
        ExtendedVertex* nextVertex = currentHalfEdge->m_next->m_vertex;

        glm::vec3 v1 = prevVertex->m_position - curVertex->m_position;
        glm::vec3 v2 = nextVertex->m_position - curVertex->m_position;

        glm::vec3 crossProduct = glm::cross(v1, v2);

        float area = 0.5f * glm::length(crossProduct);

        res.emplace_back(area);
        return res;
    }

    size_t getSize() override {
        return 1;
    }
};

class FaceHeightsFeature : public FeatureStrategy<ExtendedFace*> {
public:
    std::vector<float> invoke(ExtendedFace* obj) override {
        std::vector<float> res;

        //start of sideLengths
        std::vector<float> sideLengths;
        ExtendedHalfEdge* currentHalfEdge = obj->m_halfEdge;
        do {
            ExtendedEdge* edge = currentHalfEdge->m_edge;
            glm::vec3 v1Pos = edge->m_firstVertex->m_position;
            glm::vec3 v2Pos = edge->m_secondVertex->m_position;

            float length = glm::length(v1Pos - v2Pos);
            sideLengths.emplace_back(length);

            currentHalfEdge = currentHalfEdge->m_next;
        } while(currentHalfEdge != obj->m_halfEdge);
        //end of sideLengths

        //start of area
        currentHalfEdge = obj->m_halfEdge;

        ExtendedVertex* prevVertex = currentHalfEdge->m_previous->m_vertex;
        ExtendedVertex* curVertex  = currentHalfEdge->m_vertex;
        ExtendedVertex* nextVertex = currentHalfEdge->m_next->m_vertex;

        glm::vec3 v1 = prevVertex->m_position - curVertex->m_position;
        glm::vec3 v2 = nextVertex->m_position - curVertex->m_position;

        glm::vec3 crossProduct = glm::cross(v1, v2);

        float area = 0.5f * glm::length(crossProduct);
        //end of area

        res.emplace_back((2.0f * area) / sideLengths.at(0));
        res.emplace_back((2.0f * area) / sideLengths.at(1));
        res.emplace_back((2.0f * area) / sideLengths.at(2));

        return res;
    }

    size_t getSize() override {
        return 3;
    }
};