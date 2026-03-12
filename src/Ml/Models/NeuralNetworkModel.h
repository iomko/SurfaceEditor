#pragma once
#include <torch/script.h>

#include "DataExtractorStrategy.h"
#include "../../Scene/Mesh.h"

struct ModelInfo {
    size_t featuresSize = -1;
    std::vector<std::string> outputClassNames{};
};

class NeuralNetworkModel {
public:
    explicit NeuralNetworkModel(DataExtractorStrategyConcept* dataExtractor)
        : m_dataExtractor(dataExtractor) {
    }
    virtual ~NeuralNetworkModel() {
        delete m_dataExtractor;
    }
    virtual std::vector<float> predict(Mesh* m) = 0;
    ModelInfo getInfo() {
       return m_info;
    }

    void setModelInfo(const ModelInfo& modelInfo) {
        m_info.featuresSize = modelInfo.featuresSize;
        m_info.outputClassNames = modelInfo.outputClassNames;
    }

    ModelInfo getModelInfo() {
        return m_info;
    }

protected:
    DataExtractorStrategyConcept* m_dataExtractor;
    ModelInfo m_info;
};

class FaceSkewnessModel : public NeuralNetworkModel {
public:
    FaceSkewnessModel()
        : NeuralNetworkModel(
            [] {
                auto* extractor = new FaceDataExtractor();
                extractor->addFeatureStrategy(new FaceSideLengthsFeature());
                extractor->addFeatureStrategy(new FaceSideLengthRatioFeature());
                extractor->addFeatureStrategy(new FaceInternalAnglesFeature());
                extractor->addFeatureStrategy(new FaceAreaFeature());
                extractor->addFeatureStrategy(new FaceHeightsFeature());
                return extractor;
            }()) {
        setModelInfo(ModelInfo{static_cast<FaceDataExtractor*>(m_dataExtractor)->getFeaturesSize(), {"not_skewed", "skewed"}});
    }

    std::vector<float> predict(Mesh* mesh) override {
        std::vector<float> buffer = m_dataExtractor->invoke(mesh);

        FaceDataExtractor* dataExtractor = static_cast<FaceDataExtractor*>(m_dataExtractor);
        size_t dataSize = dataExtractor->getDataSize(mesh);

        model = torch::jit::load(std::string(ML_DATA_DIR) + "/triangle_skew_model.pt");
        model.eval();
        normMean = loadTxt(std::string(ML_DATA_DIR) + "/triangle_skew_norm_mean.txt");
        normStd  = loadTxt(std::string(ML_DATA_DIR) + "/triangle_skew_norm_std.txt");

        torch::Tensor input = torch::from_blob(
             (void*)buffer.data(),
             { static_cast<long>(dataSize), static_cast<long>(m_info.featuresSize) },
             torch::TensorOptions().dtype(torch::kFloat32)
         ).clone();

        torch::Tensor mean = torch::from_blob(normMean.data(), {static_cast<long>(getModelInfo().featuresSize)}, torch::TensorOptions().dtype(torch::kFloat32));
        torch::Tensor stdv = torch::from_blob(normStd.data(),  {static_cast<long>(getModelInfo().featuresSize)}, torch::TensorOptions().dtype(torch::kFloat32));

        input = (input - mean) / stdv;

        std::vector<torch::jit::IValue> inputs;
        inputs.push_back(input);

        torch::Tensor logits = model.forward(inputs).toTensor();

        torch::Tensor probs = torch::softmax(logits, 1).slice(1, 1, 2);
        probs = probs.contiguous();

        std::vector<float> results(dataSize);
        std::memcpy(results.data(), probs.data_ptr(), dataSize * sizeof(float));

        return results;
    }

private:
    std::vector<float> loadTxt(const std::string& path)
    {
        std::vector<float> values;
        std::ifstream in(path);

        if (!in.is_open()) {
            std::cerr << "[ML] ERROR: Cannot open " << path << "\n";
            return values;
        }

        float v;
        while (in >> v)
            values.push_back(v);

        return values;
    }

    torch::jit::script::Module model;
    std::vector<float> normMean;
    std::vector<float> normStd;
};