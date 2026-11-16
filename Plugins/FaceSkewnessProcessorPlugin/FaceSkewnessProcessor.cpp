#include "FaceSkewnessProcessor.h"

#include "../../src/Ml/Analyser/AnalyserRegistry.h"
#include "../../src/Ml/FeatureStrategies/FeatureStrategyRegistry.h"
#include "../../src/Ml/FeatureStrategies/FeatureStrategyIDS.h"
#include "../../src/Renderer/MaterialRegistry.h"

#include <algorithm>
#include <vector>

static AutoRegisterAnalyser<FaceSkewnessProcessor> regFaceSkewnessProcessor("FACE_SKEWNESS_PROCESSOR");

FaceSkewnessProcessor::FaceSkewnessProcessor() {}

std::string FaceSkewnessProcessor::getName() const {
    return NAME;
}

void FaceSkewnessProcessor::process(Mesh* mesh) {
    std::vector<FeatureStrategyConcept*> featureStrategies;

    featureStrategies.emplace_back(
        FeatureStrategyRegistry::instance().getFeature(
            FeatureStrategyIDS::FACE_SIDE_LENGTH_RATIO_FEATURE
        )
    );

    featureStrategies.emplace_back(
        FeatureStrategyRegistry::instance().getFeature(
            FeatureStrategyIDS::FACE_INTERNAL_ANGLES_FEATURE
        )
    );

    auto* ratioFeature =
        dynamic_cast<FeatureStrategy<ExtendedFace>*>(featureStrategies[0]);

    auto* anglesFeature =
        dynamic_cast<FeatureStrategy<ExtendedFace>*>(featureStrategies[1]);

    for (ExtendedFace* face : mesh->m_halfEdgeStructure->m_faces) {
        std::vector<float> ratioValue = ratioFeature->invoke(face);
        std::vector<float> angleValues = anglesFeature->invoke(face);

        float ratio = ratioValue[0];

        float minAngle = std::min({
            angleValues[0],
            angleValues[1],
            angleValues[2]
        });

        float maxAngle = std::max({
            angleValues[0],
            angleValues[1],
            angleValues[2]
        });

        bool isSkewedLabel =
            (minAngle < 10.0f) ||
            (maxAngle > 150.0f) ||
            (ratio > 10.0f);

        if (isSkewedLabel) {
            face->m_isSkewed = true;

            FaceTriangleIndex faceTriangleIndex =
                face->faceTriangleIndices.front();

            FaceTriangle& faceTriangle =
                mesh->m_halfEdgeStructure
                    ->m_faceTriangles
                    .find(face->material)
                    ->second
                    .at(faceTriangleIndex);

            if (auto opt = mesh->bufferLayout.getTriangleBufferStorage(face->material)) {
                TriangleBufferStorage& triangleBufferStorage = opt->get();
                auto& triangleBufferVertices =
                    triangleBufferStorage.data.vertices;

                int faceIndexInVao = faceTriangle.indexInVAO;

                for (int j = faceIndexInVao; j < faceIndexInVao + 3; ++j) {
                    triangleBufferVertices.at(j).isSkewed = 1.0f;
                }
            }
        }
    }

    updateFacesVaoData(mesh);
}

void FaceSkewnessProcessor::updateFacesVaoData(Mesh* mesh) {
    Material* defaultMeshMaterial =
        MaterialRegistry::getMaterial("defaultMeshMaterial");

    if (auto opt = mesh->bufferLayout.getTriangleBufferStorage(defaultMeshMaterial)) {
        TriangleBufferStorage& triangleBufferStorage = opt->get();
        triangleBufferStorage.update();
    }
}