#pragma once

#include <vector>
#include <string>
#include <torch/script.h>

#include "../../src/Ml/NeuralNetworkModels/NeuralNetworkModel.h"
#include "../../src/Ml/Analyser/AnalyserIDS.h"
#include "../../src/Ml/Editor/PredictorConfig.h"
#include "../../src/Renderer/MaterialRegistry.h"
#include "FaceMLP.h"

class Mesh;
class ExtendedFace;
class FeatureStrategyConcept;

class FaceSkewnessModel : public NeuralNetworkModel<ExtendedFace> {
public:
    static constexpr int ID = AnalyserIDS::FACE_SKEWNESS_PREDICTOR;
    static constexpr const char* NAME = "FaceSkewnessPredictor";

    FaceSkewnessModel();

    std::vector<float> predict(Mesh* mesh) override;
    void train(const std::vector<Mesh*>& meshes) override;

private:
    std::vector<float> loadTxt(const std::string& path);

    void extractDataset(
        const std::vector<Mesh*>& meshes,
        std::vector<float>& featuresOut,
        std::vector<int64_t>& labelsOut);

    std::vector<float> computeMean(
        const std::vector<float>& data,
        size_t rows,
        size_t cols);

    std::vector<float> computeStd(
        const std::vector<float>& data,
        const std::vector<float>& mean,
        size_t rows,
        size_t cols);

    void saveTxt(const std::string& path, const std::vector<float>& values);

    void updateFacesVaoData(Mesh* mesh);

private:
    bool evaluateTrainingLabel(
        ExtendedFace* face,
        const PredictorConfig& config) const;

    bool evaluateLabelDefinition(
        ExtendedFace* face,
        const LabelDefinition& label) const;

    bool evaluateCondition(
        ExtendedFace* face,
        const LabelCondition& condition) const;

    bool tryGetFeatureComponentValue(
        ExtendedFace* face,
        int featureId,
        int componentIndex,
        float& outValue) const;

    bool compareValues(float lhs, LabelOperator op, float rhs) const;

private:
    FaceMLP model;
    std::vector<float> normMean;
    std::vector<float> normStd;
};