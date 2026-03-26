#pragma once

#include <fstream>
#include <iostream>
#include <cstring>
#include <torch/script.h>

#include "NeuralNetworkModel.h"
#include "../Analyser/AnalyserRegistry.h"
#include "../Analyser/AnalyserIDS.h"
#include "../FeatureStrategies/FeatureStrategyRegistry.h"
#include "../FeatureStrategies/FaceSideLengthsFeature.h"
#include "../FeatureStrategies/FaceSideLengthRatioFeature.h"
#include "../FeatureStrategies/FaceInternalAnglesFeature.h"
#include "../FeatureStrategies/FaceAreaFeature.h"
#include "../FeatureStrategies/FaceHeightsFeature.h"
#include "FaceMLP.h"

class FaceSkewnessModel : public NeuralNetworkModel<ExtendedFace> {
public:
    static constexpr int ID = AnalyserIDS::FACE_SKEWNESS_PREDICTOR;
    static constexpr const char* NAME = "FaceSkewnessPredictor";

    FaceSkewnessModel()
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

    std::vector<float> predict(Mesh* mesh) override {
        std::vector<float> buffer;
        std::vector<ExtendedFace*> faces = mesh->m_halfEdgeStructure->m_faces;

        for (ExtendedFace* face : faces) {
            for (auto* featureStrategy : m_featureStrategies) {
                std::vector<float> feature = featureStrategy->invoke(face);
                buffer.insert(buffer.end(), feature.begin(), feature.end());
            }
        }

        size_t dataSize = faces.size();
        if (dataSize == 0) {
            return {};
        }

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
            { static_cast<long>(dataSize), static_cast<long>(m_info.featuresSize) },
            torch::TensorOptions().dtype(torch::kFloat32)
        ).clone();

        torch::Tensor mean = torch::from_blob(
            normMean.data(),
            { static_cast<long>(m_info.featuresSize) },
            torch::TensorOptions().dtype(torch::kFloat32)
        ).clone();

        torch::Tensor stdv = torch::from_blob(
            normStd.data(),
            { static_cast<long>(m_info.featuresSize) },
            torch::TensorOptions().dtype(torch::kFloat32)
        ).clone();

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
                    std::vector<BufferStorageDataType::TriangleVertex>& triangleBufferVertices =
                        triangleBufferStorage.data.vertices;

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

private:
    std::vector<float> loadTxt(const std::string& path) {
        std::vector<float> values;
        std::ifstream in(path);

        if (!in.is_open()) {
            std::cerr << "[ML] ERROR: Cannot open " << path << "\n";
            return values;
        }

        float v;
        while (in >> v) {
            values.push_back(v);
        }

        return values;
    }

private:
    FaceMLP model;
    std::vector<float> normMean;
    std::vector<float> normStd;

    void extractDataset(
    const std::vector<Mesh*>& meshes,
    std::vector<float>& featuresOut,
    std::vector<int64_t>& labelsOut)
    {
        featuresOut.clear();
        labelsOut.clear();

        for (Mesh* mesh : meshes)
        {
            if (!mesh || !mesh->m_halfEdgeStructure) {
                continue;
            }

            for (ExtendedFace* face : mesh->m_halfEdgeStructure->m_faces)
            {
                if (!face) {
                    continue;
                }

                std::vector<float> row;
                row.reserve(m_info.featuresSize);

                for (auto* featureStrategy : m_featureStrategies)
                {
                    std::vector<float> feature = featureStrategy->invoke(face);
                    row.insert(row.end(), feature.begin(), feature.end());
                }

                if (row.size() != m_info.featuresSize) {
                    std::cerr << "[ML] WARNING: Skipping face because feature size mismatch. Expected "
                              << m_info.featuresSize << ", got " << row.size() << "\n";
                    continue;
                }

                featuresOut.insert(featuresOut.end(), row.begin(), row.end());

                std::array<float, 3> angles = FeaturesExtractor::internalAngles2(face);
                float minAngle = std::min({angles[0], angles[1], angles[2]});
                float maxAngle = std::max({angles[0], angles[1], angles[2]});
                float ratio = FeaturesExtractor::ratio2(face);

                bool isSkewedLabel = ((minAngle < 10.0f) || (maxAngle > 150.0f) || ratio > 10.0f);

                labelsOut.push_back(isSkewedLabel);
            }
        }
    }

    std::vector<float> computeMean(const std::vector<float>& data, size_t rows, size_t cols)
    {
        std::vector<float> mean(cols, 0.0f);

        if (rows == 0 || cols == 0) {
            return mean;
        }

        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                mean[c] += data[r * cols + c];
            }
        }

        for (size_t c = 0; c < cols; ++c) {
            mean[c] /= static_cast<float>(rows);
        }

        return mean;
    }

    std::vector<float> computeStd(
        const std::vector<float>& data,
        const std::vector<float>& mean,
        size_t rows,
        size_t cols)
    {
        std::vector<float> stdv(cols, 0.0f);

        if (rows == 0 || cols == 0) {
            return stdv;
        }

        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                float diff = data[r * cols + c] - mean[c];
                stdv[c] += diff * diff;
            }
        }

        for (size_t c = 0; c < cols; ++c) {
            stdv[c] = std::sqrt(stdv[c] / static_cast<float>(rows)) + 1e-8f;
        }

        return stdv;
    }

    void saveTxt(const std::string& path, const std::vector<float>& values)
    {
        std::ofstream out(path);
        if (!out.is_open()) {
            std::cerr << "[ML] ERROR: Cannot save " << path << "\n";
            return;
        }

        for (float v : values) {
            out << v << "\n";
        }
    }

    void updateFacesVaoData(Mesh *mesh) {
        Material* defaultMeshMaterial = MaterialRegistry::getMaterial("defaultMeshMaterial");
        if(auto opt = mesh->bufferLayout.getTriangleBufferStorage(defaultMeshMaterial)) {
            TriangleBufferStorage& triangleBufferStorage = opt->get();
            triangleBufferStorage.update();
        }
    }

public:

    void train(const std::vector<Mesh*>& meshes) override {
        std::vector<float> rawFeatures;
        std::vector<int64_t> labels;

        extractDataset(meshes, rawFeatures, labels);

        const size_t rows = labels.size();
        const size_t cols = m_info.featuresSize;

        if (rows == 0) {
            std::cerr << "[ML] ERROR: No training samples.\n";
            return;
        }

        if (rawFeatures.size() != rows * cols) {
            std::cerr << "[ML] ERROR: Feature buffer size mismatch.\n";
            return;
        }

        normMean = computeMean(rawFeatures, rows, cols);
        normStd = computeStd(rawFeatures, normMean, rows, cols);

        // normalize in-place
        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                rawFeatures[r * cols + c] =
                    (rawFeatures[r * cols + c] - normMean[c]) / normStd[c];
            }
        }

        torch::Tensor X = torch::from_blob(
            rawFeatures.data(),
            { static_cast<long>(rows), static_cast<long>(cols) },
            torch::TensorOptions().dtype(torch::kFloat32)
        ).clone();

        torch::Tensor y = torch::from_blob(
            labels.data(),
            { static_cast<long>(rows) },
            torch::TensorOptions().dtype(torch::kInt64)
        ).clone();

        // split train/test
        torch::Tensor perm = torch::randperm(X.size(0), torch::kLong);
        X = X.index_select(0, perm);
        y = y.index_select(0, perm);

        const int64_t total = X.size(0);
        const int64_t testCount = std::max<int64_t>(1, total / 5);
        const int64_t trainCount = total - testCount;

        torch::Tensor X_train = X.narrow(0, 0, trainCount);
        torch::Tensor y_train = y.narrow(0, 0, trainCount);

        torch::Tensor X_test = X.narrow(0, trainCount, testCount);
        torch::Tensor y_test = y.narrow(0, trainCount, testCount);

        // class weights like in Python
        const int64_t numNonSkew = (y_train == 0).sum().item<int64_t>();
        const int64_t numSkew = (y_train == 1).sum().item<int64_t>();

        const float ratio = std::sqrt(
            static_cast<float>(numNonSkew) / static_cast<float>(std::max<int64_t>(1, numSkew))
        );

        torch::Tensor classWeights = torch::tensor({1.0f, ratio}, torch::kFloat32);

        model = FaceMLP(static_cast<int64_t>(cols));
        model->train();

        torch::optim::Adam optimizer(model->parameters(), torch::optim::AdamOptions(1e-3));

        const int epochs = 200;

        for (int epoch = 0; epoch < epochs; ++epoch)
        {
            optimizer.zero_grad();

            torch::Tensor logits = model->forward(X_train);
            torch::Tensor loss = torch::nn::functional::cross_entropy(
                logits,
                y_train,
                torch::nn::functional::CrossEntropyFuncOptions().weight(classWeights)
            );

            loss.backward();
            optimizer.step();

            if (epoch % 10 == 0) {
                std::cout << "Epoch " << epoch
                          << ": Loss = " << loss.item<float>() << "\n";
            }
        }

        // evaluation
        model->eval();
        {
            torch::NoGradGuard noGrad;

            torch::Tensor logits = model->forward(X_test);
            torch::Tensor predLabels = logits.argmax(1);
            torch::Tensor correct = (predLabels == y_test).to(torch::kFloat32);
            float accuracy = correct.mean().item<float>();

            std::cout << "\n----- Evaluation -----\n";
            std::cout << "Accuracy: " << accuracy << "\n";
        }

        // save model + normalization
        const std::string baseDir = std::string(ML_DATA_DIR);
        torch::save(model, baseDir + "/triangle_skew_model.pt");
        saveTxt(baseDir + "/triangle_skew_norm_mean.txt", normMean);
        saveTxt(baseDir + "/triangle_skew_norm_std.txt", normStd);

        std::cout << "[ML] Training finished and model saved.\n";
    }

private:
    inline static AutoRegisterAnalyser<FaceSkewnessModel> regFaceSkewnessModel{};
};
