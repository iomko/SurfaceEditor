#pragma once
#include <vector>
#include "../../Scene/Mesh.h"
#include "FeatureStrategy.h"

class DataExtractorStrategyConcept {
public:
    virtual ~DataExtractorStrategyConcept() = default;
    virtual std::vector<float> invoke(Mesh* mesh) = 0;
};

template<typename Type>
class DataExtractorStrategy : public DataExtractorStrategyConcept {
public:
    explicit DataExtractorStrategy(std::function<std::vector<Type>(Mesh*)> extractor)
        : m_dataExtractor(std::move(extractor)) {}

    ~DataExtractorStrategy() override {
        for (auto* feature : m_featureStrategies) {
            delete feature;
        }
    }

    std::vector<float> invoke(Mesh* mesh) final override {
        std::vector<float> res;
        const auto& objects = m_dataExtractor(mesh);

        for (auto& object : objects) {
            for (auto* featureStrategy : m_featureStrategies) {
                std::vector<float> feature = featureStrategy->invoke(object);
                res.insert(res.end(), feature.begin(), feature.end());
            }
        }

        return res;
    }

    void addFeatureStrategy(FeatureStrategy<Type>* extractor) {
        m_featureStrategies.push_back(extractor);
    }

    void removeFeatureStrategy(FeatureStrategy<Type>* extractor) {
        auto it = std::find(m_featureStrategies.begin(), m_featureStrategies.end(), extractor);

        if (it != m_featureStrategies.end()) {
            delete *it;
            m_featureStrategies.erase(it);
        }
    }

    size_t getFeaturesSize() {
        size_t retSize = 0;
        for (FeatureStrategy<Type>* featureStrategy: m_featureStrategies) {
            retSize += featureStrategy->getSize();
        }
        return retSize;
    }

    size_t getDataSize(Mesh* mesh) {
        return m_dataExtractor(mesh).size();
    }

private:
    std::vector<FeatureStrategy<Type>*> m_featureStrategies;
    std::function<std::vector<Type>(Mesh*)> m_dataExtractor;
};

class FaceDataExtractor : public DataExtractorStrategy<ExtendedFace*> {
public:
    FaceDataExtractor()
        : DataExtractorStrategy([](Mesh* mesh) -> std::vector<ExtendedFace*> {
            return mesh->m_halfEdgeStructure->m_faces;
        }) {}
};

class EdgeDataExtractor : public DataExtractorStrategy<ExtendedEdge*> {
public:
    EdgeDataExtractor()
        : DataExtractorStrategy([](Mesh* mesh) -> std::vector<ExtendedEdge*> {
            return mesh->m_halfEdgeStructure->m_edges;
        }) {}
};

class PointDataExtractor : public DataExtractorStrategy<ExtendedVertex*> {
public:
    PointDataExtractor()
        : DataExtractorStrategy([](Mesh* mesh) -> std::vector<ExtendedVertex*> {
            return mesh->m_halfEdgeStructure->m_vertices;
        }) {}
};