#pragma once
#include "FeatureStrategy.h"
#include "FeatureStrategyRegistry.h"

class FaceInternalAnglesFeature : public FeatureStrategy<ExtendedFace> {
public:
    static constexpr int ID = FeatureStrategyIDS::FACE_INTERNAL_ANGLES_FEATURE;
    static constexpr const char* NAME = "FaceInternalAnglesFeature";

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

    std::string getName() const override {
        return "FaceInternalAnglesFeature";
    }

    int getId() const override {
        return ID;
    }

private:
    inline static AutoRegisterFeatureStrategy<FaceInternalAnglesFeature> reg{};
};
