#pragma once
#include "../Patterns/Command.h"

//---CLASIC_CALLBACKS---

struct ICallback {
	virtual ~ICallback() = default;
	virtual void execute() = 0;
	virtual void execute(const Params& iParams) = 0;
	virtual void execute(const Params& iParams, Params& oParams) = 0;
};

template <typename IParams = Params, typename OParams = Params>
class Callback;

template <typename IParams, typename OParams>
class Callback : public ICallback {
public:

	virtual void execute(const IParams& iParams, OParams& oParams) = 0;
	void execute(const Params& iParams, Params& oParams) final override {
		const IParams& specificInput = static_cast<const IParams&>(iParams);
		OParams& specificOutput = static_cast<OParams&>(oParams);
		execute(specificInput, specificOutput);
	}

	void execute() final override {
		throw std::logic_error("This callback does not support execution without parameters.");
	}
	void execute(const Params&) final override {
		throw std::logic_error("This callback does not support execution with input parameters.");
	}
};

template <typename IParams>
class Callback<IParams, Params> : public ICallback {
public:

	virtual void execute(const IParams& iParams) = 0;
	void execute(const Params& iParams) final override {
		const IParams& specificParams = static_cast<const IParams&>(iParams);
		execute(specificParams);
	}

	void execute() final override {
		throw std::logic_error("This callback does not support execution without parameters.");
	}
	void execute(const Params&, Params&) final override {
		throw std::logic_error("This callback does not support execution with input and output parameters.");
	}
};

template <>
class Callback<Params, Params> : public ICallback {
public:

	virtual void execute() override = 0;

	void execute(const Params&) final override {
		throw std::logic_error("This callback does not support execution with input parameters.");
	}
	void execute(const Params&, Params&) final override {
		throw std::logic_error("This callback does not support execution with input and output parameters.");
	}
};

//---COMPOSED_CALLBACKS---


//---SIMPLE_CALLBACKS---


template <typename IParams = Params>
class SimpleCallback;

template <typename IParams>
class SimpleCallback : public ICallback {
protected:
	FunctionComposer m_composer;

public:
	explicit SimpleCallback(const FunctionComposer& functionComposer) : m_composer(functionComposer) {}

	void execute(const Params& iParams) final override {
		const IParams& specificParams = static_cast<const IParams&>(iParams);
		m_composer.execute(specificParams);
	}

	void execute() final override {
		throw std::logic_error("This callback does not support execution without parameters.");
	}
	void execute(const Params&, Params&) final override {
		throw std::logic_error("This callback does not support execution with input and output parameters.");
	}
};

template <>
class SimpleCallback<Params> : public ICallback {
protected:
	FunctionComposer m_composer;

public:
	explicit SimpleCallback(const FunctionComposer& functionComposer) : m_composer(functionComposer) {}

	void execute() final override {
		m_composer.execute();
	}

	void execute(const Params&) final override {
		throw std::logic_error("This callback does not support execution with input parameters.");
	}
	void execute(const Params&, Params&) final override {
		throw std::logic_error("This callback does not support execution with input and output parameters.");
	}
};

