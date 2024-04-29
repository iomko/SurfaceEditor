export module EventSystem.Event;
import <functional>;

export enum class EventType
{
	MouseMove, MouseButtonPress, MouseButtonRelease, MouseScroll,
	KeyPress, KeyRelease,
	WindowResize, WindowClose
};

export class Event
{
public:
	virtual ~Event() = default;
	virtual EventType getType() = 0;
	virtual const char* getName() = 0;
	bool isHandled = false;
};

export class EventDispatcher
{
public:
	EventDispatcher(Event& event) : m_event(event) {}
	template<typename T>
	void dispatch(std::function<void(T&)> eventFunc)
	{
		if (this->m_event.getType() == T::getStaticType())
		{
			eventFunc(static_cast<T&>(m_event));
		}
	}
private:
	Event& m_event;
};