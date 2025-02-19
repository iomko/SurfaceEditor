#pragma once
#include <string_view>

struct Params {
	virtual ~Params() = default;
};

class Command {
public:
	virtual ~Command() = default;

	virtual void execute() {}
	virtual void execute(const Params& params) { execute(); }
	virtual void undo() = 0;

	static constexpr std::string_view getCommandName() noexcept { return "Command"; }
};