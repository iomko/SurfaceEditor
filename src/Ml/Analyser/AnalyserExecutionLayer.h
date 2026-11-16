#pragma once

#include "Analyser.h"

class AnalyserExecutionLayer {
public:
    virtual ~AnalyserExecutionLayer() = default;

    virtual void onImGuiRender(Analyser* analyser) = 0;
};