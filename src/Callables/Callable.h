#pragma once
#include "../Patterns/Command.h"

class CallableConcept {
public:
	virtual ~CallableConcept() = default;
	virtual OpParams* execute(const OpParams& input) = 0;
};

template <int id, typename IParams, typename OParams>
class Callable : public CallableConcept {
public:
	static constexpr int ID = id; 
	using IType = IParams;
	using OType = OParams;

	OpParams* execute(const OpParams& input) final override {
		const auto& castedInput = static_cast<const IParams&>(input);
		auto output = new OParams();
		invoke(castedInput, *output);
		return output;
	}

	virtual void invoke(const IParams& input, OParams& output) = 0;
};

template <int id, typename OParams>
class Callable<id, void, OParams> : public CallableConcept {
public:
	static constexpr int ID = id; 
	using IType = void;
	using OType = OParams;

	OpParams* execute(const OpParams&) final override {
		auto output = new OParams();
		invoke(*output);
		return output;
	}
	
	virtual void invoke(OParams& output) = 0;
};

template <int id, typename IParams>
class Callable<id, IParams, void> : public CallableConcept {
public:
	static constexpr int ID = id; 
	using IType = IParams;
	using OType = void;

	OpParams* execute(const OpParams& input) final override {
		const auto& castedInput = static_cast<const IParams&>(input);
		invoke(castedInput);
		return nullptr;
	}

	virtual void invoke(const IParams& input) = 0;
};

template <int id>
class Callable<id, void, void> : public CallableConcept {
public:
	static constexpr int ID = id; 
	using IType = void;
	using OType = void;

	OpParams* execute(const OpParams&) final override {
		invoke();
		return nullptr;
	}

	virtual void invoke() = 0;
};
