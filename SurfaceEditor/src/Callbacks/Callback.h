#pragma once
#include "../FunctionComposer.h"

//---CLASIC_CALLBACKS---

struct ICallback {
	virtual ~ICallback() = default;
	virtual void execute() = 0;
	virtual void execute(const OpParams& iParams) = 0;
	virtual void execute(const OpParams& iParams, OpParams& oParams) = 0;
};

template <typename IParams = OpParams, typename OParams = OpParams>
class Callback;

template <typename IParams, typename OParams>
class Callback : public ICallback {
public:

	virtual void execute(const IParams& iParams, OParams& oParams) = 0;
	void execute(const OpParams& iParams, OpParams& oParams) final override {
		const IParams& specificInput = static_cast<const IParams&>(iParams);
		OParams& specificOutput = static_cast<OParams&>(oParams);
		execute(specificInput, specificOutput);
	}

	void execute() final override {
		throw std::logic_error("This callback does not support execution without parameters.");
	}
	void execute(const OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input parameters.");
	}
};

template <typename IParams>
class Callback<IParams, OpParams> : public ICallback {
public:

	virtual void execute(const IParams& iParams) = 0;
	void execute(const OpParams& iParams) final override {
		const IParams& specificParams = static_cast<const IParams&>(iParams);
		execute(specificParams);
	}

	void execute() final override {
		throw std::logic_error("This callback does not support execution without parameters.");
	}
	void execute(const OpParams&, OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input and output parameters.");
	}
};

template <>
class Callback<OpParams, OpParams> : public ICallback {
public:

	virtual void execute() override = 0;

	void execute(const OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input parameters.");
	}
	void execute(const OpParams&, OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input and output parameters.");
	}
};

//---COMPOSED_CALLBACKS---


//---SIMPLE_CALLBACKS---


template <typename IParams = OpParams>
class ComposedCallback;

template <typename IParams>
class ComposedCallback : public ICallback {
protected:
	FunctionComposer m_composer;

public:
	explicit ComposedCallback(const FunctionComposer& functionComposer) : m_composer(functionComposer) {}

	void execute(const OpParams& iParams) final override {
		const IParams& specificParams = static_cast<const IParams&>(iParams);
		m_composer.execute(specificParams);
	}

	void execute() final override {
		throw std::logic_error("This callback does not support execution without parameters.");
	}
	void execute(const OpParams&, OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input and output parameters.");
	}
};

template <>
class ComposedCallback<OpParams> : public ICallback {
protected:
	FunctionComposer m_composer;

public:
	explicit ComposedCallback(const FunctionComposer& functionComposer) : m_composer(functionComposer) {}

	void execute() final override {
		m_composer.execute();
	}

	void execute(const OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input parameters.");
	}
	void execute(const OpParams&, OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input and output parameters.");
	}
};

