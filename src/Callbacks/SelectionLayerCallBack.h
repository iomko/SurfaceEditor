#pragma once
#include "../Tools/FaceSelectionTool.h"
#include "../Tools/FaceDeselectionTool.h"
#include "../Tools/MeshSelectionTool.h"
#include "../Tools/MeshDeselectionTool.h"
#include "../CsvFeatureExporter.h"
#include "../ViewPortsController.h"
#include "../Ml/Models/TriangleSkewMlModel.h"

class SelectionLayerCallBack : public Callback<SELECTION_LAYER_CALLBACK, SelectionLayerParams>, public Observer
{
public:
	virtual void execute(const SelectionLayerParams& params);
};
