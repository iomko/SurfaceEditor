#pragma once
#include "../Params/OperationParams.h"
#include "../Patterns/Command.h"
#include <stdexcept>

class InteractionHandlerConcept
{
public:
	virtual ~InteractionHandlerConcept() = default;

	virtual void onBegin() = 0;
	virtual void onUpdate() = 0;
	virtual void onEnd() = 0;

	virtual void onBegin(const OpParams& iParams) = 0;
	virtual void onUpdate(const OpParams& iParams) = 0;
	virtual void onEnd(const OpParams& iParams) = 0;

	virtual CommandConcept* getCommand() const { return nullptr; }
};

template <typename CommandConcpet, typename IParams = OpParams>
class InteractionHandler;

template <typename CommandConcept>
class InteractionHandler<CommandConcept, OpParams> : public InteractionHandlerConcept {
public:
	explicit InteractionHandler(CommandConcept* command) : m_command(command) {}

	virtual void onBegin() override = 0;
	virtual void onUpdate() override = 0;
	virtual void onEnd() override = 0;

	void onBegin(const OpParams&) override final {
		throw std::logic_error("onBegin with params not supported.");
	}
	void onUpdate(const OpParams&) override final {
		throw std::logic_error("onUpdate with params not supported.");
	}
	void onEnd(const OpParams&) override final {
		throw std::logic_error("onEnd with params not supported.");
	}

	CommandConcept* getCommand() const override {
		return static_cast<CommandConcept*>(m_command);
	}

private:
	CommandConcept* m_command;
};


template <typename CommandConcept, typename IParams>
class InteractionHandler : public InteractionHandlerConcept {
public:
	explicit InteractionHandler(CommandConcept* command) : m_command(command) {}

	virtual void onBegin(const IParams&) = 0;
	virtual void onUpdate(const IParams&) = 0;
	virtual void onEnd(const IParams&) = 0;

	void onBegin(const OpParams& iParams) override final {
		onBegin(static_cast<const IParams&>(iParams));
	}
	void onUpdate(const OpParams& iParams) override final {
		onUpdate(static_cast<const IParams&>(iParams));
	}
	void onEnd(const OpParams& iParams) override final {
		onEnd(static_cast<const IParams&>(iParams));
	}

	void onBegin() override {
		throw std::logic_error("onBegin without params not supported.");
	}
	void onUpdate() override {
		throw std::logic_error("onUpdate without params not supported.");
	}
	void onEnd() override {
		throw std::logic_error("onEnd without params not supported.");
	}

	CommandConcept* getCommand() const override {
		return static_cast<CommandConcept*>(m_command);
	}

private:
	CommandConcept* m_command;
};
