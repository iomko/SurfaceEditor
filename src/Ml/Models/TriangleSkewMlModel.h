#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <torch/script.h>
#include "../../Scene/Mesh.h"
#include "MlModel.h"
#include "../../FeaturesExtractor.h"

class TriangleSkewModel : public MlModel<Mesh> {
public:
    void run(Mesh& mesh) override {
       
        auto& faces = mesh.m_halfEdgeStructure->m_faces;
        size_t N = faces.size();

        std::vector<float> buffer;
        buffer.reserve(N * 11);

        for (ExtendedFace* face : faces) {
            addFaceFeaturesToBuffer(face, buffer);
        }

        std::vector<float> probs = runBatch(buffer, N);

        for (size_t i = 0; i < N; i++)
        {
            if (probs[i] > 0.995f)
            {
                ExtendedFace* face = faces[i];
                faces[i]->m_isSkewed = true;

                std::vector<FaceTriangleIndex>& triangleIndices = face->faceTriangleIndices;

                FaceTriangleIndex faceTriangleIndex = face->faceTriangleIndices.front();
                FaceTriangle& faceTriangle = mesh.m_halfEdgeStructure->m_faceTriangles.find(face->material)->second.at(faceTriangleIndex);

                if(auto opt = mesh.bufferLayout.getTriangleBufferStorage(face->material)) {

                    TriangleBufferStorage& triangleBufferStorage = opt->get();
                    std::vector<BufferStorageDataType::TriangleVertex>& triangleBufferVertices = triangleBufferStorage.data.vertices;


                    int faceIndexInVao = faceTriangle.indexInVAO;
                    for (int i = faceIndexInVao; i < faceIndexInVao + 3; ++i)
                    {
                        triangleBufferVertices.at(i).isSkewed = 1.0f;
                    }
                }

            }
        }

        for(auto it = mesh.bufferLayout.triangleBuffersBegin(); it != mesh.bufferLayout.triangleBuffersEnd(); ++it) {
            TriangleBufferStorage& triangleBufferStorage = it->second;
            triangleBufferStorage.update();
        }

    }

    void loadModel() override
    {
        model = torch::jit::load(std::string(ML_DATA_DIR) + "/triangle_skew_model.pt");
        model.eval();

        //mali by sme mat mozno aj na vyber, ze ci chceme normalizovat data alebo nie
        normMean = loadTxt(std::string(ML_DATA_DIR) + "/triangle_skew_norm_mean.txt");
        normStd  = loadTxt(std::string(ML_DATA_DIR) + "/triangle_skew_norm_std.txt");
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

    std::vector<float> runBatch(const std::vector<float>& buffer, size_t N)
    {
        torch::Tensor input = torch::from_blob(
            (void*)buffer.data(),
            { static_cast<long>(N), 11 },
            torch::TensorOptions().dtype(torch::kFloat32)
        ).clone();

        torch::Tensor mean = torch::from_blob(normMean.data(), {11}, torch::TensorOptions().dtype(torch::kFloat32));
        torch::Tensor stdv = torch::from_blob(normStd.data(),  {11}, torch::TensorOptions().dtype(torch::kFloat32));

        input = (input - mean) / stdv;

        std::vector<torch::jit::IValue> inputs;
        inputs.push_back(input);

        torch::Tensor logits = model.forward(inputs).toTensor();

        torch::Tensor probs = torch::softmax(logits, 1).slice(1, 1, 2);
        probs = probs.contiguous();

        std::vector<float> results(N);
        std::memcpy(results.data(), probs.data_ptr(), N * sizeof(float));

        return results;
    }

    void addFaceFeaturesToBuffer(ExtendedFace* face, std::vector<float>& buffer)
    {
        auto le  = FeaturesExtractor::sideLengths2(face);
        float rt = FeaturesExtractor::ratio2(face);
        auto ang = FeaturesExtractor::internalAngles2(face);
        float ar = FeaturesExtractor::area2(face);
        auto he  = FeaturesExtractor::heights2(face);

        buffer.push_back(le[0]);
        buffer.push_back(le[1]);
        buffer.push_back(le[2]);

        buffer.push_back(rt);

        buffer.push_back(ang[0]);
        buffer.push_back(ang[1]);
        buffer.push_back(ang[2]);

        buffer.push_back(ar);

        buffer.push_back(he[0]);
        buffer.push_back(he[1]);
        buffer.push_back(he[2]);
    }

    std::vector<float> normMean;
    std::vector<float> normStd;
};
