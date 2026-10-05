#pragma once
#include "FeatureStrategy.h"
#include "FeatureStrategyRegistry.h"

class FaceAreaFeature : public FeatureStrategy<ExtendedFace> {
public:
    static constexpr int ID = FeatureStrategyIDS::FACE_AREA_FEATURE;
    static constexpr const char* NAME = "FaceAreaFeature";

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

    std::string getName() const override {
        return "FaceAreaFeature";
    }

    int getId() const override {
        return FeatureStrategyIDS::FACE_AREA_FEATURE;
    }

private:
    inline static AutoRegisterFeatureStrategy<FaceAreaFeature> reg{};
};
