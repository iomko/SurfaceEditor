#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Observer.h"


class ExportMeshesCallback : public Callback<ImportExportMeshesParams>, public Observer
{
public:
	void execute(const ImportExportMeshesParams& params) override;
};