#pragma once
#include "../Patterns/Command.h"
#include <stdexcept>
#include "../../src/Core/Profiler.h"

class CallableConcept {
public:
	virtual ~CallableConcept() = default;
	virtual OpParams* execute(const OpParams& input) = 0;
};

template <typename IParams, typename OParams>
class Callable : public CallableConcept {
public:
	using IType = IParams;
	using OType = OParams;

	OpParams* execute(const OpParams& input) final override {
		SE_PROFILE_SCOPE_N("Callable:execute");
		const auto& castedInput = static_cast<const IParams&>(input);
		auto output = new OParams();
		invoke(castedInput, *output);
		return output;
	}

	virtual void invoke(const IParams& input, OParams& output) = 0;
};

template <typename OParams>
class Callable<void, OParams> : public CallableConcept {
public:
	using IType = void;
	using OType = OParams;

	OpParams* execute(const OpParams&) final override {
		SE_PROFILE_SCOPE_N("Callable:execute");
		auto output = new OParams();
		invoke(*output);
		return output;
	}
	
	virtual void invoke(OParams& output) = 0;
};

template <typename IParams>
class Callable<IParams, void> : public CallableConcept {
public: 
	using IType = IParams;
	using OType = void;

	OpParams* execute(const OpParams& input) final override {
		SE_PROFILE_SCOPE_N("Callable:execute");
		const auto& castedInput = static_cast<const IParams&>(input);
		invoke(castedInput);
		return nullptr;
	}

	virtual void invoke(const IParams& input) = 0;
};

template <>
class Callable<void, void> : public CallableConcept {
public:
	using IType = void;
	using OType = void;

	OpParams* execute(const OpParams&) final override {
		SE_PROFILE_SCOPE_N("Callable:execute");
		invoke();
		return nullptr;
	}

	virtual void invoke() = 0;
};
