#pragma once
#include "FeatureStrategy.h"
#include "FeatureStrategyRegistry.h"

class FaceSideLengthRatioFeature : public FeatureStrategy<ExtendedFace> {
public:
    static constexpr int ID = FeatureStrategyIDS::FACE_SIDE_LENGTH_RATIO_FEATURE;
    static constexpr const char* NAME = "FaceSideLengthRatioFeature";

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

    std::string getName() const override {
        return "FaceSideLengthRatioFeature";
    }

    int getId() const override {
        return ID;
    }

private:
    inline static AutoRegisterFeatureStrategy<FaceSideLengthRatioFeature> reg{};
};
