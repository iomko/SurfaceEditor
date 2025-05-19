#pragma once

struct Params {
	virtual ~Params() = default;
};

struct ICommand {
	virtual ~ICommand() = default;
	virtual void execute() = 0;
	virtual void execute(const Params& iParams) = 0;
	virtual void execute(const Params& iParams, Params& oParams) = 0;
	virtual void undo() = 0;
};

template<typename IParams = Params, typename OParams = Params>
class Command;

template <typename IParams, typename OParams>
class Command : public ICommand
{
public:
	void undo() override {}
	void execute() override {}
	void execute(const Params& iParams) override {}

	virtual void execute(const IParams& iParams, OParams& oParams) = 0;
	void execute(const Params& iParams, Params& oParams) override
	{
		const IParams& specificIParams = static_cast<const IParams&>(iParams);
		OParams& specificOParams = static_cast<OParams&>(oParams);
		execute(specificIParams, specificOParams);
	}
};

template <typename IParams>
class Command<IParams, Params> : public ICommand {
public:
	void undo() override {}
	void execute() override{}
	void execute(const Params& iParams, Params& oParams) override {}

	virtual void execute(const IParams& iParams) = 0;
	void execute(const Params& iParams) override
	{
		const IParams& specificIParams = static_cast<const IParams&>(iParams);
		execute(specificIParams);
	}
};

template <>
class Command<Params> : public ICommand {
public:
	void undo() override {}
	void execute() override = 0;
	void execute(const Params&) override {}
	void execute(const Params& iParams, Params& oParams) override{}
};