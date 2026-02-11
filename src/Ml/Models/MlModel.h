#pragma once
#include <torch/script.h>

template <typename InputType>
class MlModel {
public:
    virtual void run(InputType& type) = 0;
    virtual void loadModel() = 0;
protected:
    torch::jit::script::Module model;
};
