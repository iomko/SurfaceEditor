export module EventSystem.Event.WindowEvent;
import EventSystem.Event;

export class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() = default;

	virtual ~WindowCloseEvent() override = default;
	virtual EventType getType() override
	{
		return EventType::WindowClose;
	}
	virtual const char* getName() override
	{
		return "windowCloseEvent";
	}
};

export class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(int width, int height)
	{
		this->width = width;
		this->height = height;
	}

	virtual ~WindowResizeEvent() override = default;
	virtual EventType getType() override
	{
		return EventType::WindowResize;
	}
	virtual const char* getName() override
	{
		return "windowResizeEvent";
	}

private:
	int width;
	int height;
};
