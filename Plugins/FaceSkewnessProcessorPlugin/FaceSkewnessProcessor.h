#pragma once

#include "../../src/Ml/Analyser/Analyser.h"
#include <string>

class FaceSkewnessProcessor : public Processor {
public:
    static constexpr const char* NAME = "FACE_SKEWNESS_PROCESSOR";

    FaceSkewnessProcessor();

    std::string getName() const override;

    void process(Mesh* mesh) override;

private:
    void updateFacesVaoData(Mesh* mesh);
};