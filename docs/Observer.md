# Observer

Implements a basic observer pattern where Observer instances listen for changes in Observable objects and get notified with optional parameters.

### Observer
Represents an entity that listens to one or more Observable objects and reacts to their notifications by executing callbacks.

Maintains a map of observed Observable pointers to callback objects.

###### Public Methods:
`void update(Observable* observable)`
Calls the stored callback with no parameters for the specified observable.

`template<typename IParams, typename OParams> void update(Observable* observable, const IParams& iParams, OParams& oParams)`
Calls the stored callback for the observable with input and output parameters.

`template<typename IParams> void update(Observable* observable, const IParams& iParams)`
Calls the stored callback with input parameters only.

`void observe(Observable* observable, CallbackConcept* callback)`
Associates the given callback with the specified observable, enabling the observer to react when notified.

### Observable
Represents a subject being observed by multiple Observer instances.

Maintains a set of observers and provides notification methods to inform them of changes.

###### Public Methods:
`void addObserver(Observer* observer)`
Adds an observer to the notification list.

`void removeObserver(Observer* observer)`
Removes an observer from the notification list.

`void notifyObservers()`
Notifies all observers without any parameters.

`void notifyObservers(const OpParams& iParams)`
Notifies all observers with input parameters.

`void notifyObservers(const OpParams& iParams, OpParams& oParams)`
Notifies all observers with input and output parameters.