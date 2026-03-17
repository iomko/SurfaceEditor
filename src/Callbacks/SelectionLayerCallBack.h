#pragma once
#include <glm/glm.hpp>
#include "../Params/OperationParams.h"
#include "CallbackIDs.h"
#include "../Patterns/Observer.h"
#include "Callback.h"
#include "../UI/SelectionRectangle.h"

class SelectionLayerCallBack : public Callback<SELECTION_LAYER_CALLBACK, SelectionLayerParams>, public Observer
{
public:
	virtual void execute(const SelectionLayerParams& params);

private:
	bool isClick(const RectanglePos& rectanglePos);

	bool isSelectedByClick(const SelectionLayerParams& params, SelectionToolParams* toolParams);

	bool isSelectedByRectangle(const RectanglePos& rectanglePos, const SelectionLayerParams& params, SelectionToolParams* toolParams);
};
