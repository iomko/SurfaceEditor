#pragma once
#include <string>
#include <vector>

#include "imgui.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Core/Layer.h"
#include "../../src/Ml/Analyser/Analyser.h"
#include "../../src/Ml/Analyser/AnalyserRegistry.h"

extern char DEBUG_LAYER[];

class DebugLayer : public Layer, public Observable, public Observer {
public:
    DebugLayer(const std::string& name);

    void onEvent(Event& event) override;
    void onImGuiRender() override;

private:
    void updateEdgesVaoData(Mesh* mesh);
    void updateFacesVaoData(Mesh* mesh);
    void unhighlightAllFaces(Mesh* mesh);

private:
    bool m_skewCheckboxState = false;
    bool m_isMouseInsideWindow = false;

    std::vector<AnalyserRegistry::InstanceInfo> m_analyserInstances;
    std::pair<std::string, std::string> m_selectedAnalyserName;
};