#pragma once
#include "FeatureStrategy.h"
#include "FeatureStrategyRegistry.h"

class FaceHeightsFeature : public FeatureStrategy<ExtendedFace> {
public:
    static constexpr int ID = FeatureStrategyIDS::FACE_HEIGHTS_FEATURE;
    static constexpr const char* NAME = "FaceHeightsFeature";

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

    std::string getName() const override {
        return "FaceHeightsFeature";
    }

    int getId() const override {
        return ID;
    }

private:
    inline static AutoRegisterFeatureStrategy<FaceHeightsFeature> reg{};
};
