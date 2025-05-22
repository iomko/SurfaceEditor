#pragma once

#include "../Params/OperationParams.h"

struct ICommand {
	virtual ~ICommand() = default;
	virtual void execute() = 0;
	virtual void execute(const OpParams& iParams) = 0;
	virtual void execute(const OpParams& iParams, OpParams& oParams) = 0;
	virtual void undo() = 0;
};

template<typename IParams = OpParams, typename OParams = OpParams>
class Command;

template <typename IParams, typename OParams>
class Command : public ICommand
{
public:
	void undo() override {}
	void execute() override {}
	void execute(const OpParams& iParams) override {}

	virtual void execute(const IParams& iParams, OParams& oParams) = 0;
	void execute(const OpParams& iParams, OpParams& oParams) override
	{
		const IParams& specificIParams = static_cast<const IParams&>(iParams);
		OParams& specificOParams = static_cast<OParams&>(oParams);
		execute(specificIParams, specificOParams);
	}
};

template <typename IParams>
class Command<IParams, OpParams> : public ICommand {
public:
	void undo() override {}
	void execute() override{}
	void execute(const OpParams& iParams, OpParams& oParams) override {}

	virtual void execute(const IParams& iParams) = 0;
	void execute(const OpParams& iParams) override
	{
		const IParams& specificIParams = static_cast<const IParams&>(iParams);
		execute(specificIParams);
	}
};

template <>
class Command<OpParams> : public ICommand {
public:
	void undo() override {}
	void execute() override = 0;
	void execute(const OpParams&) override {}
	void execute(const OpParams& iParams, OpParams& oParams) override{}
};