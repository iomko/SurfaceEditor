#pragma once
#include "../Patterns/Command.h"

class Callback {
public:
	virtual ~Callback() = default;

	virtual void execute(const Params& params) { execute(); }
	virtual void execute() {}
};