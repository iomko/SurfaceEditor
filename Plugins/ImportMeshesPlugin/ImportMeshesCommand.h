#pragma once
#include <string_view>
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"


class ImportMeshesCommand : public Observable, public Command<ImportExportMeshesParams>
{
public:
	virtual void execute(const ImportExportMeshesParams& params) override;
};
