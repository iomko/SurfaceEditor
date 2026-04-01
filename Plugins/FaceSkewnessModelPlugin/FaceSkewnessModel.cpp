#include "FaceSkewnessModel.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>

#include "../../src/Ml/Analyser/AnalyserRegistry.h"
#include "../../src/Ml/Editor/PredictorConfigDeserializer.h"
#include "../../src/Ml/FeatureStrategies/FeatureStrategyRegistry.h"
#include "../../src/Ml/FeatureStrategies/FaceSideLengthsFeature.h"
#include "../../src/Ml/FeatureStrategies/FaceSideLengthRatioFeature.h"
#include "../../src/Ml/FeatureStrategies/FaceInternalAnglesFeature.h"
#include "../../src/Ml/FeatureStrategies/FaceAreaFeature.h"
#include "../../src/Ml/FeatureStrategies/FaceHeightsFeature.h"
#include "../../src/Utils/GeometryUtils.h"

static AutoRegisterAnalyser<FaceSkewnessModel> regFaceSkewnessPredictor("FACE_SKEWNESS_PREDICTOR");

FaceSkewnessModel::FaceSkewnessModel()
    : NeuralNetworkModel<ExtendedFace>(
        NAME,
        {
            FaceSideLengthsFeature::ID,
            FaceSideLengthRatioFeature::ID,
            FaceInternalAnglesFeature::ID,
            FaceAreaFeature::ID,
            FaceHeightsFeature::ID
        }
    )
{
}

std::vector<float> FaceSkewnessModel::predict(Mesh* mesh)
{
    std::vector<float> buffer;
    std::vector<ExtendedFace*> faces = mesh->m_halfEdgeStructure->m_faces;

    for (ExtendedFace* face : faces) {
        for (auto* featureStrategy : m_featureStrategies) {
            std::vector<float> feature = featureStrategy->invoke(face);
            buffer.insert(buffer.end(), feature.begin(), feature.end());
        }
    }

    size_t dataSize = faces.size();
    if (dataSize == 0) return {};

    normMean = loadTxt(std::string(ML_DATA_DIR) + "/triangle_skew_norm_mean.txt");
    normStd  = loadTxt(std::string(ML_DATA_DIR) + "/triangle_skew_norm_std.txt");

    if (normMean.size() != m_info.featuresSize || normStd.size() != m_info.featuresSize) {
        std::cerr << "[ML] ERROR: normalization vectors have invalid size.\n";
        return {};
    }

    model = FaceMLP(static_cast<int64_t>(m_info.featuresSize));
    torch::load(model, std::string(ML_DATA_DIR) + "/triangle_skew_model.pt");
    model->eval();

    torch::Tensor input = torch::from_blob(
        buffer.data(),
        { (long)dataSize, (long)m_info.featuresSize },
        torch::kFloat32
    ).clone();

    torch::Tensor mean = torch::from_blob(normMean.data(), { (long)m_info.featuresSize }, torch::kFloat32).clone();
    torch::Tensor stdv = torch::from_blob(normStd.data(),  { (long)m_info.featuresSize }, torch::kFloat32).clone();

    input = (input - mean) / stdv;

    torch::NoGradGuard noGrad;
    torch::Tensor logits = model->forward(input);
    torch::Tensor probs = torch::softmax(logits, 1).slice(1, 1, 2).contiguous();

    std::vector<float> results(dataSize);
    std::memcpy(results.data(), probs.data_ptr<float>(), dataSize * sizeof(float));

    for (size_t i = 0; i < results.size(); i++)
    {
        if (results[i] > 0.995f)
        {
            ExtendedFace* face = mesh->m_halfEdgeStructure->m_faces[i];
            face->m_isSkewed = true;

            FaceTriangleIndex faceTriangleIndex = face->faceTriangleIndices.front();
            FaceTriangle& faceTriangle =
                mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second.at(faceTriangleIndex);

            if (auto opt = mesh->bufferLayout.getTriangleBufferStorage(face->material)) {
                TriangleBufferStorage& triangleBufferStorage = opt->get();
                auto& triangleBufferVertices = triangleBufferStorage.data.vertices;

                int faceIndexInVao = faceTriangle.indexInVAO;
                for (int j = faceIndexInVao; j < faceIndexInVao + 3; ++j) {
                    triangleBufferVertices.at(j).isSkewed = 1.0f;
                }
            }
        }
    }

    for (auto it = mesh->bufferLayout.triangleBuffersBegin();
         it != mesh->bufferLayout.triangleBuffersEnd(); ++it)
    {
        TriangleBufferStorage& triangleBufferStorage = it->second;
        triangleBufferStorage.update();
    }

    updateFacesVaoData(mesh);

    return results;
}

std::vector<float> FaceSkewnessModel::loadTxt(const std::string& path)
{
    std::vector<float> values;
    std::ifstream in(path);

    if (!in.is_open()) {
        std::cerr << "[ML] ERROR: Cannot open " << path << "\n";
        return values;
    }

    float v;
    while (in >> v) values.push_back(v);

    return values;
}

bool FaceSkewnessModel::compareValues(float lhs, LabelOperator op, float rhs) const
{
    switch (op)
    {
        case LabelOperator::Equal:        return lhs == rhs;
        case LabelOperator::NotEqual:     return lhs != rhs;
        case LabelOperator::Less:         return lhs < rhs;
        case LabelOperator::LessEqual:    return lhs <= rhs;
        case LabelOperator::Greater:      return lhs > rhs;
        case LabelOperator::GreaterEqual: return lhs >= rhs;
        default:                          return false;
    }
}

bool FaceSkewnessModel::tryGetFeatureComponentValue(
    ExtendedFace* face,
    int featureId,
    int componentIndex,
    float& outValue) const
{
    if (!face) {
        return false;
    }

    FeatureStrategyConcept* featureConcept =
        FeatureStrategyRegistry::instance().getFeature(featureId);

    if (!featureConcept) {
        return false;
    }

    if (featureConcept->getObjectType() != getFeatureObjectType()) {
        return false;
    }

    auto* typedFeature = dynamic_cast<FeatureStrategy<ExtendedFace>*>(featureConcept);
    if (!typedFeature) {
        return false;
    }

    std::vector<float> values = typedFeature->invoke(face);

    if (componentIndex < 0 || componentIndex >= static_cast<int>(values.size())) {
        return false;
    }

    outValue = values[componentIndex];
    return true;
}

bool FaceSkewnessModel::evaluateCondition(
    ExtendedFace* face,
    const LabelCondition& condition) const
{
    float featureValue = 0.0f;
    if (!tryGetFeatureComponentValue(face, condition.featureId, condition.componentIndex, featureValue)) {
        return false;
    }

    return compareValues(featureValue, condition.op, condition.value);
}

bool FaceSkewnessModel::evaluateLabelDefinition(
    ExtendedFace* face,
    const LabelDefinition& label) const
{
    if (label.conditions.empty()) {
        return false;
    }

    if (label.mode == LabelMatchMode::All)
    {
        for (const LabelCondition& condition : label.conditions)
        {
            if (!evaluateCondition(face, condition)) {
                return false;
            }
        }

        return true;
    }

    if (label.mode == LabelMatchMode::Any)
    {
        for (const LabelCondition& condition : label.conditions)
        {
            if (evaluateCondition(face, condition)) {
                return true;
            }
        }

        return false;
    }

    return false;
}

bool FaceSkewnessModel::evaluateTrainingLabel(
    ExtendedFace* face,
    const PredictorConfig& config) const
{
    if (config.labels.empty()) {
        return false;
    }

    // Current FaceSkewnessModel is a binary classifier.
    // We use the first label definition in config as the positive class.
    return evaluateLabelDefinition(face, config.labels.front());
}

void FaceSkewnessModel::extractDataset(
    const std::vector<Mesh*>& meshes,
    std::vector<float>& featuresOut,
    std::vector<int64_t>& labelsOut)
{
    featuresOut.clear();
    labelsOut.clear();

    PredictorConfig config = PredictorConfigDeserializer::loadFromFile(getModelConfigPath());

    if (config.labels.empty())
    {
        std::cerr << "[ML] WARNING: No labels defined in config for " << getName()
                  << ". Training labels will default to 0.\n";
    }

    if (config.labels.size() > 1)
    {
        std::cerr << "[ML] WARNING: " << getName()
                  << " is currently a binary classifier. Only the first label in config will be used for training.\n";
    }

    for (Mesh* mesh : meshes)
    {
        if (!mesh || !mesh->m_halfEdgeStructure) continue;

        for (ExtendedFace* face : mesh->m_halfEdgeStructure->m_faces)
        {
            if (!face) continue;

            std::vector<float> row;
            row.reserve(m_info.featuresSize);

            for (auto* featureStrategy : m_featureStrategies)
            {
                std::vector<float> feature = featureStrategy->invoke(face);
                row.insert(row.end(), feature.begin(), feature.end());
            }

            if (row.size() != m_info.featuresSize) continue;

            featuresOut.insert(featuresOut.end(), row.begin(), row.end());

            const bool positiveLabel = evaluateTrainingLabel(face, config);
            labelsOut.push_back(positiveLabel ? 1 : 0);
        }
    }
}

std::vector<float> FaceSkewnessModel::computeMean(
    const std::vector<float>& data, size_t rows, size_t cols)
{
    std::vector<float> mean(cols, 0.0f);

    for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols; ++c)
            mean[c] += data[r * cols + c];

    for (float& m : mean)
        m /= static_cast<float>(rows);

    return mean;
}

std::vector<float> FaceSkewnessModel::computeStd(
    const std::vector<float>& data,
    const std::vector<float>& mean,
    size_t rows,
    size_t cols)
{
    std::vector<float> stdv(cols, 0.0f);

    for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols; ++c)
        {
            float diff = data[r * cols + c] - mean[c];
            stdv[c] += diff * diff;
        }

    for (float& s : stdv)
        s = std::sqrt(s / static_cast<float>(rows)) + 1e-8f;

    return stdv;
}

void FaceSkewnessModel::saveTxt(const std::string& path, const std::vector<float>& values)
{
    std::ofstream out(path);
    if (!out.is_open()) return;

    for (float v : values)
        out << v << "\n";
}

void FaceSkewnessModel::updateFacesVaoData(Mesh *mesh)
{
    Material* defaultMeshMaterial = MaterialRegistry::getMaterial("defaultMeshMaterial");
    if(auto opt = mesh->bufferLayout.getTriangleBufferStorage(defaultMeshMaterial)) {
        TriangleBufferStorage& triangleBufferStorage = opt->get();
        triangleBufferStorage.update();
    }
}

void FaceSkewnessModel::train(const std::vector<Mesh*>& meshes)
{
    std::cout << "START TRAINING MODEL" << std::endl;

    std::vector<float> rawFeatures;
    std::vector<int64_t> labels;

    extractDataset(meshes, rawFeatures, labels);

    size_t rows = labels.size();
    size_t cols = m_info.featuresSize;

    if (rows == 0) return;

    normMean = computeMean(rawFeatures, rows, cols);
    normStd  = computeStd(rawFeatures, normMean, rows, cols);

    for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols; ++c)
            rawFeatures[r * cols + c] =
                (rawFeatures[r * cols + c] - normMean[c]) / normStd[c];

    torch::Tensor X = torch::from_blob(rawFeatures.data(), { (long)rows, (long)cols }, torch::kFloat32).clone();
    torch::Tensor y = torch::from_blob(labels.data(), { (long)rows }, torch::kInt64).clone();

    model = FaceMLP((int64_t)cols);
    model->train();

    torch::optim::Adam optimizer(model->parameters(), 1e-3);

    for (int epoch = 0; epoch < 200; ++epoch)
    {
        optimizer.zero_grad();
        auto loss = torch::nn::functional::cross_entropy(model->forward(X), y);
        loss.backward();
        optimizer.step();
    }

    torch::save(model, std::string(ML_DATA_DIR) + "/triangle_skew_model.pt");
    saveTxt(std::string(ML_DATA_DIR) + "/triangle_skew_norm_mean.txt", normMean);
    saveTxt(std::string(ML_DATA_DIR) + "/triangle_skew_norm_std.txt", normStd);

    std::cout << "DONE TRAINING MODEL" << std::endl;
}