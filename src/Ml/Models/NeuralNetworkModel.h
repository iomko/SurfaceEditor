#pragma once
#include <torch/script.h>
#include "../../Scene/Mesh.h"

class NeuralNetworkModel {
public:
    virtual void load(const std::string& neuralNetworkModelPath) = 0;
    virtual void predict(Mesh* mesh) = 0;
    void setDataExtractor(DataExtractorStrategyConcept* dataExtractor) {
        m_dataExtractor = dataExtractor;
    }
    virtual void train() = 0;
    virtual void save() = 0;
private:
    DataExtractorStrategyConcept* m_dataExtractor;
    torch::jit::script::Module m_neuralNetworkModel;
};
