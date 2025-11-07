#include "Observer.h"

void Observer::update(Observable *observable)
{
    m_observers[observable]->execute();
}

template <typename IParams, typename OParams>
void Observer::update(Observable *observable, const IParams &iParams, OParams &oParams)
{
    m_observers[observable]->execute(iParams, oParams);
}

template <typename IParams>
void Observer::update(Observable *observable, const IParams &iParams)
{
    m_observers[observable]->execute(iParams);
}
void Observer::observe(Observable *observable, CallbackConcept *callback)
{
    m_observers[observable] = callback;
}

void Observable::addObserver(Observer *observer)
{
    m_observers.insert(observer);
}
void Observable::removeObserver(Observer *observer)
{
    m_observers.erase(observer);
}

void Observable::notifyObservers()
{
    for (const auto &observer : m_observers)
    {
        observer->update(this);
    }
}

void Observable::notifyObservers(const OpParams &iParams)
{
    for (const auto &observer : m_observers)
    {
        observer->update(this, iParams);
    }
}

void Observable::notifyObservers(const OpParams &iParams, OpParams &oParams)
{
    for (const auto &observer : m_observers)
    {
        observer->update(this, iParams, oParams);
    }
}