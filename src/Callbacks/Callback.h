#pragma once
#include "../Callables/FunctionComposer.h"
#include <memory>
#include <stdexcept>

struct CallbackConcept {
	virtual ~CallbackConcept() = default;
	virtual void execute() = 0;
	virtual void execute(const OpParams& iParams) = 0;
	virtual void execute(const OpParams& iParams, OpParams& oParams) = 0;
};

template <int id, typename IParams = OpParams, typename OParams = OpParams>
class Callback;

template <int id, typename IParams, typename OParams>
class Callback : public CallbackConcept {
public:
	static constexpr int ID = id; 

	virtual void execute(const IParams& iParams, OParams& oParams) = 0;
	void execute(const OpParams& iParams, OpParams& oParams) final override {
		const IParams& castedInput = static_cast<const IParams&>(iParams);
		OParams& castedOutput = static_cast<OParams&>(oParams);
		execute(castedInput, castedOutput);
	}

	void execute() final override {
		throw std::logic_error("This callback does not support execution without any parameters.");
	}
	void execute(const OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input parameters.");
	}
};

template <int id, typename IParams>
class Callback<id, IParams, OpParams> : public CallbackConcept {
public:
	static constexpr int ID = id; 

	virtual void execute(const IParams& iParams) = 0;
	void execute(const OpParams& iParams) final override {
		const IParams& castedIParams = static_cast<const IParams&>(iParams);
		execute(castedIParams);
	}

	void execute() final override {
		throw std::logic_error("This callback does not support execution without any parameters.");
	}
	void execute(const OpParams&, OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input and output parameters.");
	}
};

template <int id>
class Callback<id, OpParams, OpParams> : public CallbackConcept {
public:
	static constexpr int ID = id; 

	virtual void execute() = 0;

	void execute(const OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input parameters.");
	}
	void execute(const OpParams&, OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input and output parameters.");
	}
};


template <int id, typename IParams = OpParams>
class ComposedCallback;

template <int id, typename IParams>
class ComposedCallback : public CallbackConcept {
public:
	static constexpr int ID = id; 
protected:
	std::unique_ptr<FunctionComposer>m_composer;

public:
	explicit ComposedCallback(std::unique_ptr<FunctionComposer> functionComposer) : m_composer(std::move(functionComposer)) {}
	
	void execute(const OpParams& iParams) final override {
		const IParams& castedIParams = static_cast<const IParams&>(iParams);
		m_composer->execute(castedIParams);
	}

	void execute() final override {
		throw std::logic_error("This callback does not support execution without any parameters.");
	}
	void execute(const OpParams&, OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input and output parameters.");
	}
};

template <int id>
class ComposedCallback<id, OpParams> : public CallbackConcept {
public:
	static constexpr int ID = id; 
protected:
	std::unique_ptr<FunctionComposer> m_composer;

public:
	explicit ComposedCallback(std::unique_ptr<FunctionComposer> functionComposer) : m_composer(std::move(functionComposer)) {}
	
	void execute() final override {
		m_composer->execute();
	}

	void execute(const OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input parameters.");
	}
	void execute(const OpParams&, OpParams&) final override {
		throw std::logic_error("This callback does not support execution with input and output parameters.");
	}
};

