#pragma once
#include <torch/torch.h>

struct FaceMLPImpl : torch::nn::Module {
    torch::nn::Linear fc1{nullptr};
    torch::nn::Linear fc2{nullptr};
    torch::nn::Linear fc3{nullptr};

    FaceMLPImpl(int64_t inputSize = 11)
    {
        fc1 = register_module("fc1", torch::nn::Linear(inputSize, 64));
        fc2 = register_module("fc2", torch::nn::Linear(64, 32));
        fc3 = register_module("fc3", torch::nn::Linear(32, 2));
    }

    torch::Tensor forward(torch::Tensor x)
    {
        x = torch::relu(fc1->forward(x));
        x = torch::relu(fc2->forward(x));
        x = fc3->forward(x);
        return x;
    }
};

TORCH_MODULE(FaceMLP);