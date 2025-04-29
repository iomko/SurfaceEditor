#pragma once

struct Params {
	virtual ~Params() = default;
};

struct ICommand {
	virtual ~ICommand() = default;
	virtual void execute() = 0;
	virtual void execute(const Params& params) = 0;
	virtual void undo() = 0;
};

template<typename IParams = Params>
class Command;

template<typename IParams>
class Command : public ICommand
{
public:
	void undo() override {}
	void execute() override {}
	void execute(const Params& params) override {
		const IParams& specificParams = static_cast<const IParams&>(params);
		execute(specificParams);
	}
	virtual void execute(const IParams& params) = 0;
};

template <>
class Command<Params> : public ICommand {
public:
	void undo() override {}
	void execute() override = 0;
	void execute(const Params&) override {}
};