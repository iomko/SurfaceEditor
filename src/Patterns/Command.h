#pragma once

#include "../Params/OperationParams.h"
#include <stdexcept>

struct CommandConcept
{
	virtual ~CommandConcept() = default;
	virtual void execute() = 0;
	virtual void execute(const OpParams &iParams) = 0;
	virtual void execute(const OpParams &iParams, OpParams &oParams) = 0;
	virtual void undo() = 0;
};

template <int id, typename IParams = OpParams, typename OParams = OpParams>
class Command;

template <int id, typename IParams, typename OParams>
class Command : public CommandConcept
{
public:
	static constexpr int ID = id; 
	void undo() override {}
	void execute() override {}
	virtual void execute(const OpParams &iParams) override {}

	virtual void execute(const IParams &iParams, OParams &oParams) = 0;
	void execute(const OpParams &iParams, OpParams &oParams) override
	{
		const IParams &specificIParams = static_cast<const IParams &>(iParams);
		OParams &specificOParams = static_cast<OParams &>(oParams);
		execute(specificIParams, specificOParams);
	}
};

template <int id, typename IParams>
class Command<id, IParams, OpParams> : public CommandConcept
{
public:
	static constexpr int ID = id;
	void undo() override {}
	void execute() override {}
	void execute(const OpParams &iParams, OpParams &oParams) override {}

	virtual void execute(const IParams &iParams) = 0;
	void execute(const OpParams &iParams) override
	{
		const IParams &specificIParams = static_cast<const IParams &>(iParams);
		execute(specificIParams);
	}
};

template <int id>
class Command<id, OpParams> : public CommandConcept
{
public:
	static constexpr int ID = id;
	void undo() override {}
	void execute() override = 0;
	void execute(const OpParams &) override {}
	void execute(const OpParams &iParams, OpParams &oParams) override {}
};