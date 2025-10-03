# Observer

Implements a basic observer pattern where Observer instances listen for changes in Observable objects and get notified with optional parameters.  

### Observer class
Represents an entity that listens to one or more Observable objects and reacts to their notifications by executing callbacks.  

###### Methods:
`void update(Observable* observable)` - Calls the stored callback with no parameters for the specified observable.  

`template<typename IParams, typename OParams> void update(Observable* observable, const IParams& iParams, OParams& oParams)` - Calls the stored callback for the observable with input and output parameters.  

`template<typename IParams> void update(Observable* observable, const IParams& iParams)` - Calls the stored callback with input parameters only.  

`void observe(Observable* observable, CallbackConcept* callback)` - Associates the given callback with the specified observable, enabling the observer to react when notified.  

### Observable class
Represents a subject being observed by multiple Observer instances.  

###### Methods:
`void addObserver(Observer* observer)` - Adds an observer to the observers list.  

`void removeObserver(Observer* observer)` - Removes an observer from the observers list.  

`void notifyObservers()` - Notifies all observers without any parameters.  

`void notifyObservers(const OpParams& iParams)` - Notifies all observers with input parameters.  

`void notifyObservers(const OpParams& iParams, OpParams& oParams)` - Notifies all observers with input and output parameters.  
