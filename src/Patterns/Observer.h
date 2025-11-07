#pragma once
#include <set>
#include <map>

#include "Command.h"
#include "../Callbacks/Callback.h"

class Observable;

class Observer {
public:

	void update(Observable* observable);

	template<typename IParams, typename OParams>
	void update(Observable* observable, const IParams& iParams, OParams& oParams);

	template<typename IParams>
	void update(Observable* observable, const IParams& iParams);
	void observe(Observable* observable, CallbackConcept* callback);
private:
	std::map<Observable*, CallbackConcept*> m_observers;
};

class Observable {
public:

	void addObserver(Observer* observer);
	void removeObserver(Observer* observer);

	void notifyObservers();

	void notifyObservers(const OpParams& iParams);

	void notifyObservers(const OpParams& iParams, OpParams& oParams);
public:
	std::set<Observer*> m_observers;
};
