export module Patterns.Observer;

export template <typename T>
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(const T& data) = 0;
};

export template <typename T>
class IObservable {
public:
    virtual ~IObservable() = default;
    virtual void addObserver(IObserver<T>* observer) = 0;
    virtual void removeObserver(IObserver<T>* observer) = 0;
    virtual void notifyObservers(const T& data) = 0;
};