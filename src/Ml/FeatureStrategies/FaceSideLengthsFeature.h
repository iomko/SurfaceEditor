#pragma once
#include "FeatureStrategy.h"
#include "FeatureStrategyRegistry.h"

class FaceSideLengthsFeature : public FeatureStrategy<ExtendedFace> {
public:
    static constexpr int ID = FeatureStrategyIDS::FACE_SIDE_LENGTHS_FEATURE;
    static constexpr const char* NAME = "FaceSideLengthsFeature";

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

    std::string getName() const override {
        return "FaceSideLengthsFeature";
    }

    int getId() const override {
        return ID;
    }

private:
    inline static AutoRegisterFeatureStrategy<FaceSideLengthsFeature> reg{};
};
