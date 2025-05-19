#pragma once
#include <set>
#include <map>

#include "Command.h"
#include "../Callbacks/Callback.h"


class Observable;

class Observer {
public:

	void update(Observable* observable)
	{
		m_observers[observable]->execute();
	}

	template<typename IParams, typename OParams>
	void update(Observable* observable, const IParams& iParams, OParams& oParams)
	{
		m_observers[observable]->execute(iParams, oParams);
	}

	template<typename IParams>
	void update(Observable* observable, const IParams& iParams)
	{
		m_observers[observable]->execute(iParams);
	}
	void observe(Observable* observable, ICallback* callback)
	{
		m_observers[observable] = callback;
	}
private:
	std::map<Observable*, ICallback*> m_observers;
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
		for (const auto& observer : m_observers)
		{
			observer->update(this);
		}
	}

	void notifyObservers(const Params& iParams)
	{
		for (const auto& observer : m_observers)
		{
			observer->update(this, iParams);
		}
	}

	void notifyObservers(const Params& iParams, Params& oParams)
	{
		for (const auto& observer : m_observers)
		{
			observer->update(this, iParams, oParams);
		}
	}
public:
	std::set<Observer*> m_observers;
};

/*
class Observable;

class Observer {
public:
	void update(Observable* observable) {
		m_observers[observable].execute();
	}

	void update(Observable* observable, const Params& observableParams) {
		m_observers[observable].execute(observableParams);
	}

	template <typename Callable>
	void observe(Observable* observable, Callable&& callable) {
		m_observers[observable] = Callback(std::forward<Callable>(callable));
	}

private:
	std::map<Observable*, Callback> m_observers;
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
		for (const auto& observer : m_observers)
		{
			observer->update(this);
		}
	}

	void notifyObservers(const Params& observableParams)
	{
		for (const auto& observer : m_observers)
		{
			observer->update(this, observableParams);
		}
	}
public:
	std::set<Observer*> m_observers;
};
*/