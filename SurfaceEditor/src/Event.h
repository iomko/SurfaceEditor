#ifndef EVENT_H
#define EVENT_H

#include <functional>

enum class EventType
{
	MouseMove, MouseButtonPress, MouseButtonRelease, MouseScroll,
	KeyPress, KeyRelease,
	WindowResize, WindowClose
};

class Event
{
public:
	virtual ~Event() = default;
	virtual EventType getType() = 0;
	virtual const char* getName() = 0;
	bool isHandled = false;
};

class EventDispatcher
{
public:
	EventDispatcher(Event& event) : event(event){}
	template<typename T>
	void dispatch(std::function<void(T&)> eventFunc)
    {
		if(this->event.getType() == T::getStaticType())
		{
			eventFunc(static_cast<T&>(event));
		}
    }
private:
	Event& event;
};
#endif
