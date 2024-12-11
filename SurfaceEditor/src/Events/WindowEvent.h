#pragma once
#include "Event.h"

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() = default;

	virtual ~WindowCloseEvent() override = default;

	static EventType getStaticType()
	{
		return EventType::WindowClose;
	}

	virtual EventType getType() override
	{
		return EventType::WindowClose;
	}
	virtual const char* getName() override
	{
		return "windowCloseEvent";
	}
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(int width, int height)
	{
		this->width = width;
		this->height = height;
	}

	virtual ~WindowResizeEvent() override = default;

	static EventType getStaticType()
	{
		return EventType::WindowResize;
	}

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
