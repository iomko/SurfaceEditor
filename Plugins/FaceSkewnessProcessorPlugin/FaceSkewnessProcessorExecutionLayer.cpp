#include "FaceSkewnessProcessorExecutionLayer.h"
#include "FaceSkewnessProcessor.h"

#include "../../src/Ml/Analyser/AnalyserExecutionLayerRegistry.h"
#include "../../src/ViewPortsController.h"

#include "imgui.h"

static AutoRegisterAnalyserExecutionLayer<FaceSkewnessProcessorExecutionLayer>
    regFaceSkewnessProcessorExecutionLayer(FaceSkewnessProcessor::NAME);

void FaceSkewnessProcessorExecutionLayer::onImGuiRender(Analyser* analyser) {
    auto* processor = dynamic_cast<FaceSkewnessProcessor*>(analyser);

    if (!processor) {
        ImGui::Text("Invalid analyser selected.");
        return;
    }

    ImGui::Separator();
    ImGui::Text("Face skewness processor settings");

    static float minAngleThreshold = 10.0f;
    static float maxAngleThreshold = 150.0f;
    static float ratioThreshold = 10.0f;

    ImGui::DragFloat("Min angle threshold", &minAngleThreshold, 0.1f);
    ImGui::DragFloat("Max angle threshold", &maxAngleThreshold, 0.1f);
    ImGui::DragFloat("Ratio threshold", &ratioThreshold, 0.1f);

    if (ImGui::Button("Process selected meshes")) {
        SelectionController* selectionController =
            ViewPortsHolderContext::s_selectionController.get();

        const SelectionHolder& selectionHolder =
            selectionController->getHolder();

        for (Mesh* mesh : selectionHolder.meshes) {
            processor->process(mesh);
        }
    }
}