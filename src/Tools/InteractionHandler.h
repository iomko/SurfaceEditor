#pragma once
#include "../Patterns/Command.h"
#include <stdexcept>

#include "../Core/Profiler.h"

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
		SE_PROFILE_SCOPE_N("InteractionHandler:onBegin");
		throw std::logic_error("onBegin with params not supported.");
	}
	void onUpdate(const OpParams&) override final {
		SE_PROFILE_SCOPE_N("InteractionHandler:onUpdate");
		throw std::logic_error("onUpdate with params not supported.");
	}
	void onEnd(const OpParams&) override final {
		SE_PROFILE_SCOPE_N("InteractionHandler:onEnd");
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
		SE_PROFILE_SCOPE_N("InteractionHandler:onBegin");
		onBegin(static_cast<const IParams&>(iParams));
	}
	void onUpdate(const OpParams& iParams) override final {
		SE_PROFILE_SCOPE_N("InteractionHandler:onUpdate");
		onUpdate(static_cast<const IParams&>(iParams));
	}
	void onEnd(const OpParams& iParams) override final {
		SE_PROFILE_SCOPE_N("InteractionHandler:onEnd");
		onEnd(static_cast<const IParams&>(iParams));
	}

	void onBegin() override {
		SE_PROFILE_SCOPE_N("InteractionHandler:onBegin");
		throw std::logic_error("onBegin without params not supported.");
	}
	void onUpdate() override {
		SE_PROFILE_SCOPE_N("InteractionHandler:onUpdate");
		throw std::logic_error("onUpdate without params not supported.");
	}
	void onEnd() override {
		SE_PROFILE_SCOPE_N("InteractionHandler:onEnd");
		throw std::logic_error("onEnd without params not supported.");
	}

	CommandConcept* getCommand() const override {
		return static_cast<CommandConcept*>(m_command);
	}

private:
	CommandConcept* m_command;
};
