export module Patterns.Observers;
import <utility>;

export template <typename... Args>
class IObservers {
public:
    virtual ~IObservers() = default;
    virtual void update(Args&&... args) = 0;
};

export template <typename... Observers>
class IObservables : public Observers... {
public:
    template <typename... Args>
    void notifyObservers(Args&&... args) {
        (this->Observers::update(std::forward<Args>(args)), ...);
    }
};