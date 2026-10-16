#pragma once
#include <set>
#include <map>

#include "Command.h"
#include "../Callbacks/Callback.h"

#include "../../src/Core/Profiler.h"

class Observable;

class Observer {
public:

	void update(Observable* observable)
	{
		SE_PROFILE_SCOPE_N("Observer:update");
		m_observers[observable]->execute();
	}

	template<typename IParams, typename OParams>
	void update(Observable* observable, const IParams& iParams, OParams& oParams)
	{
		SE_PROFILE_SCOPE_N("Observer:update");
		m_observers[observable]->execute(iParams, oParams);
	}

	template<typename IParams>
	void update(Observable* observable, const IParams& iParams)
	{
		SE_PROFILE_SCOPE_N("Observer:update");
		m_observers[observable]->execute(iParams);
	}

	virtual void update(Observable* observable, const OpParams& iParams)
	{
		SE_PROFILE_SCOPE_N("Observer:update");
		m_observers[observable]->execute(iParams);
	}

	void observe(Observable* observable, CallbackConcept* callback)
	{
		m_observers[observable] = callback;
	}
private:
	std::map<Observable*, CallbackConcept*> m_observers;
};

class Observable {
public:

	void addObserver(Observer* observer)
	{
		m_observers.insert(observer);
	}
	void removeObserver(Observer* observer)
	{
		m_observers.erase(observer);
	}

	void notifyObservers()
	{
		SE_PROFILE_SCOPE_N("Observable:notifyObservers");
		for (const auto& observer : m_observers)
		{
			observer->update(this);
		}
	}

	void notifyObservers(const OpParams& iParams)
	{
		SE_PROFILE_SCOPE_N("Observable:notifyObservers");
		for (const auto& observer : m_observers)
		{
			observer->update(this, iParams);
		}
	}

	virtual void notifyObservers(const OpParams& iParams, OpParams& oParams)
	{
		SE_PROFILE_SCOPE_N("Observable:notifyObservers");
		for (const auto& observer : m_observers)
		{
			observer->update(this, iParams, oParams);
		}
	}
public:
	std::set<Observer*> m_observers;
};
