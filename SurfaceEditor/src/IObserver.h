#pragma once
class IObserver
{
public:
	virtual ~IObserver() = default;
	virtual void updateFromObservable() = 0;
};