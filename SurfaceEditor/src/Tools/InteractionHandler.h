#pragma once
#include "../Params/OperationParams.h"

class IInteractionHandler
{
public:
	virtual ~IInteractionHandler() = default;

	virtual void onBegin() {}
	virtual void onUpdate() {}
	virtual void onEnd() {}

	virtual void onBegin(const OpParams& iParams) {}
	virtual void onUpdate(const OpParams& iParams) {}
	virtual void onEnd(const OpParams& iParams) {}
};

template <typename IParams = OpParams>
class InteractionHandler : public IInteractionHandler {
public:
	virtual void onBegin(const IParams& iParams) = 0;
	virtual void onUpdate(const IParams& iParams) = 0;
	virtual void onEnd(const IParams& iParams) = 0;

	void onBegin(const OpParams& iParams) override final {
		const IParams& specificParams = static_cast<const IParams&>(iParams);
		onBegin(specificParams);
	}

	void onUpdate(const OpParams& iParams) override final {
		const IParams& specificParams = static_cast<const IParams&>(iParams);
		onUpdate(specificParams);
	}

	void onEnd(const OpParams& iParams) override final {
		const IParams& specificParams = static_cast<const IParams&>(iParams);
		onEnd(specificParams);
	}

	void onBegin() override final {throw std::logic_error("InteractionHandler does not support onBegin without parameters.");}
	void onUpdate() override final {throw std::logic_error("InteractionHandler does not support onUpdate without parameters.");}
	void onEnd() override final {throw std::logic_error("InteractionHandler does not support onEnd without parameters.");}
};

template <>
class InteractionHandler<OpParams> : public IInteractionHandler {
public:
	virtual void onBegin() override = 0;
	virtual void onUpdate() override = 0;
	virtual void onEnd() override = 0;

	void onBegin(const OpParams& iParams) override final {throw std::logic_error("InteractionHandler does not support onBegin with parameters.");}
	void onUpdate(const OpParams& iParams) override final {throw std::logic_error("InteractionHandler does not support onUpdate with parameters.");}
	void onEnd(const OpParams& iParams) override final {throw std::logic_error("InteractionHandler does not support onEnd with parameters.");}
};