#pragma once
#include "../Patterns/Command.h"


class CallableBase {
public:
	virtual ~CallableBase() = default;

	virtual Params* execute(const Params& input) = 0;
};

template <typename TInputParams, typename TOutputParams>
class Callable : public CallableBase {
public:
	using InputType = TInputParams;
	using OutputType = TOutputParams;

	Params* execute(const Params& input) final override {
		const auto& in = static_cast<const TInputParams&>(input);
		auto out = new TOutputParams();
		this->invoke(in, *out);
		return out;
	}

	virtual void invoke(const TInputParams& input, TOutputParams& output) = 0;
};

template <typename TOutputParams>
class Callable<void, TOutputParams> : public CallableBase {
public:
	using InputType = void;
	using OutputType = TOutputParams;

	Params* execute(const Params&) final override {
		auto out = new TOutputParams();
		this->invoke(*out);
		return out;
	}

	virtual void invoke(TOutputParams& output) = 0;
};

template <typename TInputParams>
class Callable<TInputParams, void> : public CallableBase {
public:
	using InputType = TInputParams;
	using OutputType = void;

	Params* execute(const Params& input) final override {
		const auto& in = static_cast<const TInputParams&>(input);
		this->invoke(in);
		return nullptr;
	}

	virtual void invoke(const TInputParams& input) = 0;
};

template <>
class Callable<void, void> : public CallableBase {
public:
	using InputType = void;
	using OutputType = void;

	Params* execute(const Params&) final override {
		this->invoke();
		return nullptr;
	}

	virtual void invoke() = 0;
};