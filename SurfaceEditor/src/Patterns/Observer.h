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

    void update(Observable* observable, const Params& observableParams)
    {
        m_observers[observable]->execute(observableParams);
    }
    void observe(Observable* observable, Callback* callback)
    {
	    m_observers[observable] = callback;
	}
private:
    std::map<Observable*, Callback*> m_observers;
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
	    for(const auto& observer : m_observers)
	    {
            observer->update(this);
	    }
    }

    void notifyObservers(const Params& observableParams)
    {
        for(const auto& observer : m_observers)
        {
            observer->update(this, observableParams);
        }
    }
public:
    std::set<Observer*> m_observers;
};