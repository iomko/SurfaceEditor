#pragma once
#include <string_view>
#include <vector>
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"

#include "../ViewPortsHolder.h"
#include "CmdProperties/CmdProperties.h"

class ImportMeshesCommand : public Command<ImportExportMeshesParams>, public Observable
{
public:
	virtual void execute(const ImportExportMeshesParams& params) override
	{
		notifyObservers(params);
	}

	static constexpr std::string_view getCommandName() noexcept { return "ImportMeshesCommand"; }
};

/*
class ImportMeshesCommand : public Observable, public Command {
public:
	virtual void execute(const Params& params) override {
		notifyObservers(params);
	}

	void undo() override {
	}

	static constexpr std::string_view getCommandName() noexcept { return "ImportMeshesCommand"; }
};
*/