#pragma once

#include "../../src/Ml/Analyser/AnalyserExecutionLayer.h"

class FaceSkewnessProcessorExecutionLayer : public AnalyserExecutionLayer {
public:
    void onImGuiRender(Analyser* analyser) override;
};