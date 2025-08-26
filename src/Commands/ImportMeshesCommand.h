#pragma once
#include <string_view>
#include "Params/OperationParams.h"
#include "Patterns/Command.h"
#include "Patterns/Observer.h"


class ImportMeshesCommand : public Command<ImportExportMeshesParams>, public Observable
{
public:
	virtual void execute(const ImportExportMeshesParams& params) override
	{
		notifyObservers(params);
	}

	static constexpr std::string_view getCommandName() noexcept { return "ImportMeshesCommand"; }
};
