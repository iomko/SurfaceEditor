#pragma once
#include <string_view>
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "../ViewPortsHolder.h"
#include "CmdProperties/CmdProperties.h"

class TransformMeshCommand : public Observable, public Command<TransformMeshParams> {
public:
	virtual void execute(const TransformMeshParams& params) override {
		notifyObservers(params);
	}

	void undo() override {
	}

	static constexpr std::string_view getCommandName() noexcept { return "TransformMeshCommand"; }
};