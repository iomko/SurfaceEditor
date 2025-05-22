#pragma once

class ImportMeshesCommand : public Command<ImportExportMeshesParams>, public Observable
{
public:
	virtual void execute(const ImportExportMeshesParams& params) override
	{
		notifyObservers(params);
	}

	static constexpr std::string_view getCommandName() noexcept { return "ImportMeshesCommand"; }
};