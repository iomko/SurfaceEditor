#pragma once
#include "IObserver.h"
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual void add(IObserver* observer) = 0;
	virtual void notify() = 0;
};